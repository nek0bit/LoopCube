#include "game.hpp"

Game::Game(Timer& timer, int argc, char** argv)
    : argc{argc},
      argv{argv},
      title{"LoopCube"},
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
    game = nullptr;
    menu = nullptr;
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
	
	//menu = std::make_shared<Menu>(renderer, *textures, events, timer, winSize);
}

// Game related loop stuff
void Game::update()
{
#if 0
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
                if (argc >= 3)
                {
                    try
                    {
                        std::string address = argv[1];
                        uint16_t port = std::stoi(std::string(argv[2]));
                        
                        game = std::make_shared<GameClient>(address, port, timer, winSize);
                    }
                    catch (const std::invalid_argument& err)
                    {
                        isRunning = false;
                        std::cerr << "[Error] Port must be a valid number" << std::endl;
                        return;
                    }
                    catch (const ConnectionError& err)
                    {
                        isRunning = false;
                        std::cerr << "[Error] Couldn't connect to server: " << err.what() << std::endl;
                        return;
                    }
                }
                else
                {
                    game = std::make_shared<GameClient>(timer, winSize);
                }
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

    if (game != nullptr && game->exit)
    {
        game = nullptr;
        state.pop(); // Switch game state to menu
    }
#endif

	// Update screen size
    SDL_GetWindowSize(window, &winSize.w, &winSize.h);
    
}

// Draw objects to screen
void Game::render() {
#if 0
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
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
    
    SDL_RenderPresent(renderer);
#endif

#if defined(__WIIU__) || defined(__SWITCH__)
	SDL_Rect cursor_hover{events.vmouse.x, events.vmouse.y, 10, 10};

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
	SDL_RenderFillRect(renderer, &cursor_hover);
#endif


    SDL_GL_SwapWindow(window);
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

    // Setup flags
    win_flags |= SDL_WINDOW_OPENGL;
    
    if (fullscreen) win_flags |= SDL_WINDOW_FULLSCREEN;
    win_flags |= SDL_WINDOW_RESIZABLE;
    
    //rend_flags = rend_flags | SDL_RENDERER_ACCELERATED;
    //rend_flags = rend_flags | SDL_RENDERER_PRESENTVSYNC;	

    SDL_StartTextInput();

    // Initialize OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Initialize GLAD
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // Initialize SDL2 Window
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) == 0) {
        window = SDL_CreateWindow(title,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  defWidth, defHeight, win_flags);

        // Create OpenGL context
        context = SDL_GL_CreateContext(window);
    } else {
        char error[] = "SDL_Init failed: ";
        strcat(error, SDL_GetError());
        throw std::runtime_error(error);
    }


    //textures = std::make_shared<TextureHandler>(renderer);

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
        SDL_Quit();
        SDL_GL_DeleteContext(context);
	}

	hasFreed = true;
}
