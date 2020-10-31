#include "game.hpp"

enum GAME_STATE {
	// Menu
	STATE_MAIN_MENU,

	// Game
	STATE_PLAYING,
} STATE;

Game::Game() : title{"LoopCube"}, state{}, game{nullptr} {

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
	
	menu = new Menu(renderer, events);
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
			game = new Play(renderer, events);
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
	renderer->fetch_screen_size();
}



// Draw objects to screen
void Game::render() {
	renderer->clear_screen();

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
	
#if defined(__WIIU__) || defined(__SWITCH__)
	Rect cursor_hover{events->get_mouse_pos()[0], events->get_mouse_pos()[1], 10, 10};

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
	SDL_RenderFillRect(renderer, &cursor_hover);
	Color cursor_color{255, 255, 255, 200};
	renderer->render_rect(cursor_hover, cursor_color);
#endif

	renderer->update_screen();
}

// SDL2 related stuff below
// Initiates Window/Game Loop
void Game::init(bool fullscreen = false) {
	fullscreen = true; // Bypass unused variable temporarily
	
#ifdef GRAPHIC_BACKEND_SDL2
	renderer = new GraphicsWrapper_SDL2(Config{});
#endif

#ifdef GRAPHIC_BACKEND_SFML
	renderer = new GraphicsWrapper_SFML(Config{});
#endif

#ifdef INPUT_BACKEND_SDL2
    events = new EventWrapper_SDL2();
#endif

#ifdef INPUT_BACKEND_SFML
	events = new EventWrapper();
#endif

	try {
		renderer->init_screen();
	} catch(std::runtime_error& e) {
		std::cout << "[Error] " << e.what() << std::endl;
	    exit(1);
	}

	events->update_controllers();

	game_init();

	is_running = true;
}

// Handles events such as exit, keypresses, mouse
void Game::event_handler() {
	events->listen();

	auto eventer = events->get_key_state();

	if (events->get_quit()) {
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
		delete events;
		delete renderer;
	}

	has_freed = true;
}
