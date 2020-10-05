#include "game.hpp"

enum GAME_STATE {
    // Menu
    STATE_MAIN_MENU,
    STATE_CONFIG,

    // Game
    STATE_PLAYING,
    STATE_PAUSE,
    STATE_INVENTORY,
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
    textures = TextureHandler(renderer);
    //game = new Play(renderer, textures, events, WINDOW_W, WINDOW_H);
    menu = new Menu(renderer, textures, events, &WINDOW_W, &WINDOW_H, {"Play", "About", "Settings", "Exit"});
}

// Game related loop stuff
void Game::update() {
    if (state == STATE_MAIN_MENU) {
        menu->update();
        if (menu->get_pressed() == 0) {
            state.set(STATE_PLAYING);
        }
        if (menu->get_pressed() == 1) {
            std::cout << "Don't worry, about coming soon!" << std::endl;
        }
        if (menu->get_pressed() == 2) {
            std::cout << "Don't worry, settings coming soon!" << std::endl;
        }
        if (menu->get_pressed() == 3) {
            is_running = false;
        }
    }
    if (state == STATE_PLAYING) {
        // Check if the game is nullptr, then create it
        if (game == nullptr) {
            game = new Play(renderer, textures, events, &WINDOW_W, &WINDOW_H);
        }
        game->update();
    }

    // Update screen size
    SDL_GetWindowSize(window, &WINDOW_W, &WINDOW_H);
}



// Draw objects to screen
void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0x79, 0xae, 0xd9, 255);
    SDL_RenderClear(renderer);

    // game->render();
    if (state == STATE_MAIN_MENU) {
        menu->render();
    }
    if (state == STATE_PLAYING) {
        game->render();
    }
    
#if defined(__WIIU__) || defined(__SWITCH__)
    SDL_Rect cursor_hover;
    cursor_hover.x = events.get_mouse_pos()[0];
    cursor_hover.y = events.get_mouse_pos()[1];
    cursor_hover.w = 10;
    cursor_hover.h = 10;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &cursor_hover);
#endif

    SDL_RenderPresent(renderer);
}

// SDL2 related stuff below
// Initiates Window/Game Loop
void Game::init(bool fullscreen = false) {
    // Handle flags
    int flags = 0;
#if defined(__WIIU__) || defined(__SWITCH__)
    fullscreen = true;
#endif
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    flags = flags | SDL_WINDOW_RESIZABLE;
    #if !defined(__WIIU__)
    flags = flags | SDL_RENDERER_ACCELERATED;
    #endif
    
    SDL_StartTextInput();
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "[SDL] Initialized SDL" << std::endl;
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, flags);
        renderer = SDL_CreateRenderer(window, -1, 0);
        SDL_SetRenderDrawColor(renderer, 0x79, 0xae, 0xd9, 255);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    } else {
        std::cerr << "[SDL] ERROR: Failed to initialize SDL!" << std::endl;
    }

    events.open_controllers();

    // Todo check error
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    initialize_fonts();

    game_init();

    is_running = true;
}

void Game::initialize_fonts() {
    constants::button_font = TTF_OpenFont("data/fonts/liberation-sans/LiberationSans-Regular.ttf", 26);
    constants::item_font = TTF_OpenFont("data/fonts/liberation-sans/LiberationSans-Regular.ttf", 12);
	constants::header_font = TTF_OpenFont("data/fonts/liberation-sans/LiberationSans-Regular.ttf", 32);
	constants::paragraph_font = TTF_OpenFont("data/fonts/liberation-sans/LiberationSans-Regular.ttf", 16);
}

// Handles events such as exit, keypresses, mouse
void Game::event_handler() {

    events.listen();

    auto eventer = events.get_state();

    if (events.get_quit()) {
        is_running = false;
    }
}

bool Game::running() {
    return is_running;
}

void Game::free() {
    // Incase user manually runs this method and then the destructor calls this afterwards
    if (has_freed == false) {
        std::cout << "[Game] Cleaning up..." << std::endl;
        //delete game;
        delete menu;
        std::cout << "Closing SDL_TextInput..." << std::endl;
        SDL_StopTextInput();
        std::cout << "[Textures] Freeing..." << std::endl;
        textures.free_textures();
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
    }

    has_freed = true;
}
