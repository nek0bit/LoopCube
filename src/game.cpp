#include "game.hpp"

Game::Game(Timer& timer)
    : title{"LoopCube"},
      state{},
      game{nullptr},
      menu{nullptr},
      winSize{},
      timer{timer},
      textures{nullptr}
{
    state.push(STATE_MAIN_MENU);
}

Game::~Game()
{
	// Clean up all game objects/SDL stuff to prevent memory leakage
	free();
}

// Game related stuff below
// Initiates Game objects
void Game::gameInit()
{
	// Setup config
	constants::config.set(CONFIG_SHOW_SHADOWS, true);
	constants::config.set(CONFIG_SHOW_PARTICLES, true);
	constants::config.set(CONFIG_LOAD_DISTANCE, 12);
	constants::config.set(CONFIG_SHOW_CHUNK_DEBUG, false);

    // Setup fonts
    constants::fontHandler.addFontByFilename(constants::rootPath+"fonts/liberation-sans/LiberationSans-Regular.ttf",
                                             {10, 12, 14, 16, 18, 32});
	
	menu = std::shared_ptr<Menu>(new Menu(renderer, *textures, events, timer, winSize));
}

// Game related loop stuff
void Game::update()
{
    if (state.size() > 0)
    {
        switch(state.top()) {
        case STATE_MAIN_MENU:
            menu->update();
            if (game != nullptr && menu->showPlayBuffer == false)
            {
                menu->showPlayBuffer = true;
            }
            switch(menu->getPressed()) {
            case 0:
                // Set state, we need to redo this switch case afterwards
                state.push(STATE_PLAYING);
			
                // Go through switch-case again so we can setup the game
                update();
			
                break;
            case 1:
                // NOTE not implemented yet
                break;
            case 2:
                menu->setState(1);
                break;
            case 3:
                isRunning = false;
                break;
            default:
                break;
            }
            break;
        case STATE_PLAYING:
            // Check if the game is nullptr, then create it
            if (game == nullptr) {
                game = std::shared_ptr<GameClient>(new GameClient("127.0.0.1", 8726, timer, winSize));
                // Let's pre-load a frame so everything can generate and render
                // This may need to change depending on world generation in the future
                game->update(events);
            } else {
                game->update(events);
            }
            break;
        default:
            break;
        }
    }

	// Update screen size
    SDL_GetWindowSize(window, &winSize.w, &winSize.h);
}



// Draw objects to screen
void Game::render() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (state.size() > 0)
    {
        switch(state.top()) {
        case STATE_MAIN_MENU:
            if (game != nullptr) game->render(renderer, *textures, events);
            if (menu != nullptr) menu->render();
            break;
        case STATE_PLAYING:
            if (game != nullptr) game->render(renderer, *textures, events);
            break;
        default:
            break;
        }
    }

    // TODO clean me up
#if defined(__WIIU__) || defined(__SWITCH__)
	SDL_Rect cursor_hover{events.vmouse.x, events.vmouse.y, 10, 10};

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
	SDL_RenderFillRect(renderer, &cursor_hover);
#endif

    SDL_RenderPresent(renderer);
}

// SDL2 related stuff below
// Initiates Window/Game Loop
void Game::init(bool fullscreen = false) {
	int win_flags = 0;
	int rend_flags = 0;

    int defWidth = 800;
    int defHeight = 600;

#ifdef __SWITCH__
    defWidth = 1280;
    defHeight = 720;
#endif
    
	if (fullscreen) win_flags = win_flags | SDL_WINDOW_FULLSCREEN;
	win_flags = win_flags | SDL_WINDOW_RESIZABLE;

	rend_flags = rend_flags | SDL_RENDERER_ACCELERATED;
	//rend_flags = rend_flags | SDL_RENDERER_PRESENTVSYNC;	

    SDL_StartTextInput();
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) == 0) {
		window = SDL_CreateWindow(title,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  defWidth, defHeight, win_flags);
		renderer = SDL_CreateRenderer(window, -1, rend_flags);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // Enable alpha blending
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	} else {
		char error[] = "SDL_Init failed: ";
		strcat(error, SDL_GetError());
		throw std::runtime_error(error);
	}

    textures = std::shared_ptr<TextureHandler>(new TextureHandler(renderer));

    // Enable images
	int img_flags = IMG_INIT_PNG;
	if ((IMG_Init(img_flags) & img_flags) != img_flags) {
		char error[] = "IMG_Init failed: ";
		strcat(error, IMG_GetError());
		throw std::runtime_error(error);
	}

    TTF_Init();

	events.updateControllers();

	gameInit();

	isRunning = true;
}

// Handles events such as exit, keypresses, mouse
void Game::eventHandler() {
	events.listen();

	if (events.quit)
    {
		isRunning = false;
	}

    if (events.keyState[17])
    {
        state.pop();
        if (state.size() == 0)
        {
            events.quit = true;
        }
    }
}

void Game::free() {
	// Incase user manually runs this method and then the destructor calls this afterwards
	if (!hasFreed) {
        SDL_StopTextInput();
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
	}

	hasFreed = true;
}
