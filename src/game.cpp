#include "game.hpp"

Game::Game(Timer& timer, int argc, char** argv)
    : camZoom{1.0f, 1.0f},
      camZoomRes{1.0f, 1.0f},
      argc{argc},
      argv{argv},
      title{"LoopCube"},
      state{},
      game{nullptr},
      menu{nullptr},
      winSize{},
      timer{timer},
      graphics{nullptr, winSize}
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
    updateTitle();
    
	// Setup config
	constants::config.set(CONFIG_SHOW_SHADOWS, true);
	constants::config.set(CONFIG_SHOW_PARTICLES, true);
	constants::config.set(CONFIG_LOAD_DISTANCE, 12);
	constants::config.set(CONFIG_SHOW_CHUNK_DEBUG, false);

    // Setup fonts
    constants::fontHandler.addFontByFilename(static_cast<std::string>(constants::rootPath) +
                                             "fonts/liberation-sans/LiberationSans-Regular.ttf",
                                             {10, 12, 14, 16, 18, 32});

    // Create menu
	menu = std::make_unique<Menu>(graphics);
    menu->onPlayClick = [&]() {
        state.push(STATE_PLAYING);
    };
    
    createModels();
}

void Game::createModels()
{
    const texcoord_info pModInfo = {
        constants::textureInfo[TEXTURE_PLAYER].sizeX,
        constants::textureInfo[TEXTURE_PLAYER].sizeY,
        constants::textureInfo[TEXTURE_PLAYER].tilemapX,
        constants::textureInfo[TEXTURE_PLAYER].tilemapY
    };
    
    const texcoord_t pModGet = Texture::getTilemapCoord(pModInfo, 0, 0);
    
    // SQUARE
    graphics.models.addModel(graphics.shader, {
            {{0.0f, 1.0f, 0.0f},  {0.0f, 0.0f}},
            {{1.0f, 1.0f, 0.0f},  {1.0f, 0.0f}},
            {{1.0f, 0.0f, 0.0f},  {1.0f, 1.0f}},
            
            {{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
            {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}}
        });
    
    // PLAYER
    graphics.models.addModel(graphics.shader, {
            {{0.0f, 1.0f, 0.0f},  {pModGet.begX, 1.0f}},
            {{1.0f, 1.0f, 0.0f},  {pModGet.endX, 1.0f}},
            {{1.0f, 0.0f, 0.0f},  {pModGet.endX, 0.0f}},
            
            {{1.0f, 0.0f, 0.0f}, {pModGet.endX, 0.0f}},
            {{0.0f, 0.0f, 0.0f}, {pModGet.begX, 0.0f}},
            {{0.0f, 1.0f, 0.0f}, {pModGet.begX, 1.0f}}
        });
}

// Draw objects to screen
void Game::render() {
    // Clear screen
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glScissor(0, 0, graphics.camera.size.w, graphics.camera.size.h);
    glClear(GL_COLOR_BUFFER_BIT);

    if (state.size() > 0)
    {
        switch(state.top()) {
        case STATE_MAIN_MENU:
            //if (game != nullptr) game->render(graphics, events, camZoomRes);
            if (menu != nullptr) menu->render(graphics);
            break;
        case STATE_PLAYING:
            if (game != nullptr) game->render(graphics, events, camZoomRes);
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


    // Update and bind camera
    graphics.camera.updateProj();
    graphics.camera.bindProj(graphics.uniforms.proj);
    
    graphics.camera.updateView();
    graphics.camera.bindView(graphics.uniforms.view);
    
    SDL_GL_SwapWindow(graphics.window);
}

void Game::handleFPS()
{
    tickTime += timer.deltaTime;

    // 1 second
    if (static_cast<int>(tickTime) % 10)
    {
        updateTitle();
        tickTime = 0;
    }
}

void Game::updateTitle()
{
    using namespace std::string_literals;
    
    std::string title = "LoopCube ["s + std::to_string(static_cast<int>(timer.fps)) + " fps]";
    if (graphics.window != nullptr) SDL_SetWindowTitle(graphics.window, title.c_str());
}

// Game related loop stuff
void Game::update()
{
    handleFPS();
    
    // Update screen size
    SDL_GetWindowSize(graphics.window, &winSize.w, &winSize.h);
    
    graphics.camera.position.z = 100.0f;

    if (state.size() > 0)
    {
        switch(state.top()) {
        case STATE_MAIN_MENU:
            graphics.camera.position = {0.0f, 0.0f, 100.0f};
            graphics.camera.center = {0.0f, 0.0f, 0.0f};
            if (menu == nullptr) break;
            menu->update(graphics.camera, events);
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
                        
                        game = std::make_unique<GameClient>(graphics.shader, address, port, timer, winSize);
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
                    game = std::make_unique<GameClient>(graphics.shader, timer, winSize);
                }
                // Next we preload a frame
            }
            game->update(graphics.camera, events, camZoomRes);
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

    glViewport(0, 0, winSize.w, winSize.h);
}

// SDL2 related stuff below
// Initiates Window/Game Loop
void Game::init(bool fullscreen = false) {
    int win_flags = 0;

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
    
    SDL_StartTextInput();


    // Initialize SDL2 Window
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) == 0) {
        // Initialize OpenGL options
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        
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
        throw std::runtime_error("Failed to initialize GLAD");

    // Disable VSYNC
    SDL_GL_SetSwapInterval(0);

    graphics.init();

    // Shader is compiled and used
    graphics.loadShaders(static_cast<std::string>(constants::shaderPath) + "vertex.glsl",
                         static_cast<std::string>(constants::shaderPath) + "frag.glsl");

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
    graphics.textures.init();

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
