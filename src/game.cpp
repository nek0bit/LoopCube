#include "game.hpp"

enum GAME_STATE {
	// Menu
	STATE_MAIN_MENU,

	// Game
	STATE_PLAYING,
} STATE;

Game::Game() : title{"LoopCube"}, state{}, game{nullptr}, menu{nullptr}, winSize{}, textures{nullptr} {

}

Game::~Game() {
	// Clean up all game objects/SDL stuff to prevent memory leakage
	free();
}

// Game related stuff below
// Initiates Game objects
void Game::game_init() {
	// Setup config
	constants::config.set(CONFIG_SHOW_SHADOWS, 1);
	constants::config.set(CONFIG_SHOW_PARTICLES, 1);
	constants::config.set(CONFIG_LOAD_DISTANCE, 12);
	constants::config.set(CONFIG_SHOW_CHUNK_DEBUG, 0);

    // Setup fonts
    constants::fontHandler.addFontByFilename(constants::root_path+"fonts/liberation-sans/LiberationSans-Regular.ttf",
                                             {10, 12, 14, 16, 18, 32});
	
	menu = new Menu(renderer, *textures, events, winSize);
}

// Game related loop stuff
void Game::update() {
	switch(state.get()) {
	case STATE_MAIN_MENU:
		menu->update();
		switch(menu->get_pressed()) {
		case 0:
			// Set state, we need to redo this switch case afterwards
			state.set(STATE_PLAYING);
			
			// Go through switch-case again so we can setup the game
			update();
			
			break;
		case 1:
			// NOTE not implemented yet
			break;
		case 2:
			menu->set_state(1);
			break;
		case 3:
			is_running = false;
			break;
		default:
			break;
		}
		break;
	case STATE_PLAYING:
		// Check if the game is nullptr, then create it
		if (game == nullptr) {
			game = new Play(renderer, *textures, events, winSize);
			// Let's pre-load a frame so everything can generate and render
			// This may need to change depending on world generation in the future
			game->update();
		} else {
			game->update();
		}
		break;
	default:
		break;
	}

	// Update screen size
    SDL_GetWindowSize(window, &winSize.w, &winSize.h);
}



// Draw objects to screen
void Game::render() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

	switch(state.get()) {
	case STATE_MAIN_MENU:
		if (menu != nullptr) menu->render();
		break;
	case STATE_PLAYING:
		if (game != nullptr) game->render();
		break;
	default:
		break;
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
    
	if (fullscreen) win_flags = win_flags | SDL_WINDOW_FULLSCREEN;
	win_flags = win_flags | SDL_WINDOW_RESIZABLE;

	rend_flags = rend_flags | SDL_RENDERER_ACCELERATED;
	//rend_flags = rend_flags | SDL_RENDERER_PRESENTVSYNC;	

	if (SDL_Init(SDL_INIT_VIDEO) == 0) {
		window = SDL_CreateWindow(title,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  800, 600, win_flags);
		renderer = SDL_CreateRenderer(window, -1, rend_flags);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // Enable alpha blending
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	} else {
		char error[] = "SDL_Init failed: ";
		strcat(error, SDL_GetError());
		throw std::runtime_error(error);
	}

    textures = new TextureHandler(renderer); 

    // Enable images
	int img_flags = IMG_INIT_PNG;
	if ((IMG_Init(img_flags) & img_flags) != img_flags) {
		char error[] = "IMG_Init failed: ";
		strcat(error, IMG_GetError());
		throw std::runtime_error(error);
	}

    TTF_Init();

	events.update_controllers();

	game_init();

	is_running = true;
}

// Handles events such as exit, keypresses, mouse
void Game::event_handler() {
	events.listen();

	if (events.quit) {
		is_running = false;
	}
}

bool Game::running() {
	return is_running;
}

void Game::free() {
	// Incase user manually runs this method and then the destructor calls this afterwards
	if (has_freed == false) {
		// Cleanup core game stuff
		delete game;
		delete menu;

        // SDL2 Cleanup
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
	}

	has_freed = true;
}
