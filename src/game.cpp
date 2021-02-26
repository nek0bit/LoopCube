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
      graphics{nullptr},
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
	
	menu = std::make_shared<Menu>(graphics, *textures, events, timer, winSize);

    // REMOVE ME
    /*
      // Kept for reference
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    float vertices[] = {
        // Position            Texture Coords
        -0.5f,  0.5f, 0.0f,    0.0f, 0.0f,
         0.5f,  0.5f, 0.0f,    1.0f, 0.0f,
         0.5f, -0.5f, 0.0f,    1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,    0.0f, 1.0f
    };
    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    graphics.setupVertexLayout();
    */

}


// Draw objects to screen
void Game::render() {
    // Clear screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    
    textures->getTexture(2)->bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    

    
    if (state.size() > 0)
    {
        switch(state.top()) {
        case STATE_MAIN_MENU:
            //if (game != nullptr) game->render(graphics, *textures, events);
            if (menu != nullptr) menu->render();
            break;
        case STATE_PLAYING:
            //if (game != nullptr) game->render(graphics, *textures, events);
            break;
        default:
            break;
        }
    }
    
#if defined(__WIIU__) || defined(__SWITCH__)
	SDL_Rect cursor_hover{events.vmouse.x, events.vmouse.y, 10, 10};

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
	SDL_RenderFillRect(renderer, &cursor_hover);
#endif

    SDL_GL_SwapWindow(graphics.window);
}


// Game related loop stuff
void Game::update()
{
    // Update screen size
    SDL_GetWindowSize(graphics.window, &winSize.w, &winSize.h);
    
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

    // Initialize OpenGL options
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Initialize SDL2 Window
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) == 0) {
        graphics.window = SDL_CreateWindow(title,
                                           SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED,
                                           defWidth, defHeight, win_flags);

        // Create OpenGL context
        graphics.context = SDL_GL_CreateContext(graphics.window);
    } else {
        char error[] = "SDL_Init failed: ";
        strcat(error, SDL_GetError());
        throw std::runtime_error(error);
    }
    
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // Initialize OpenGL calls
    graphics.init();

    // Shader is compiled and used
    graphics.loadShaders(constants::shaderPath + "vertex.glsl",
                         constants::shaderPath + "frag.glsl");

    // Initiates images and fonts
    int img_flags = IMG_INIT_PNG;
    if ((IMG_Init(img_flags) & img_flags) != img_flags) {
        char error[] = "IMG_Init failed: ";
        strcat(error, IMG_GetError());
        throw std::runtime_error(error);
    }
    TTF_Init();

    // Setup any connected gamepads
    events.updateControllers();

    // Load textures on startup
    textures = std::make_shared<TextureHandler>();

    // Start game!
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
        SDL_Quit();
	}

	hasFreed = true;
}
