#include "gameclient.hpp"

GameClient::GameClient(const GLuint shader, Timer& timer, WinSize& winSize)
    : exit{false},
      server{nullptr},
      clientSocket{nullptr},
      disconnectCallback{},
      winSize{winSize},
      serverPlayers{},
      serverChunks{shader},
      mainPlayer{},
      entities{},
      fade{60},
      playerPosUpdate{6},
      time{8600, 28500, 8600, 22000, 1700, 1700},
      timer{timer},
      background{std::make_shared<BackgroundOverworld>()},
      particleGroup{},
      particleModel{shader}
{    
    try
    {
        // Create server
        server = std::make_shared<Server>(8726);
        serverThread = std::thread(&GameClient::serverThreadFunction, this);
    }
    catch (const std::exception& err)
    {
        std::cout << "Exception: " << err.what() << std::endl;
        server = nullptr;
    }

    try
    {
        // Connect to server
        clientSocket = std::make_shared<ClientSocket>(nullptr, 8726);
        serverChunks.setFd(clientSocket->fd);
    }
    catch (const std::exception& err)
    {
        std::cout << "Error connecting to server: " << err.what() << std::endl;
    }

    init();
}

GameClient::GameClient(const GLuint shader, std::string address, uint16_t port, Timer& timer, WinSize& winSize)
    : exit{false},
      server{nullptr},
      clientSocket{nullptr},
      serverThread{},
      winSize{winSize},
      serverPlayers{},
      serverChunks{shader},
      mainPlayer{},
      entities{},
      fade{60},
      playerPosUpdate{6},
      time{8600, 28500, 8600, 22000, 1700, 1700},
      timer{timer},
      background{std::make_shared<BackgroundOverworld>()},
      particleGroup{},
      particleModel{shader}
{
    clientSocket = std::make_shared<ClientSocket>(address.c_str(), port);
    serverChunks.setFd(clientSocket->fd);

    init();
}

GameClient::~GameClient()
{
    server.reset();
    server = nullptr;

    if (serverThread.joinable()) serverThread.join();
}

void GameClient::init()
{
    disconnectCallback = [&]()->void {
        exit = true;
        std::cout << "[Debug] Connection closed" << std::endl;
    };

    particleModel.setBufferData({
            {{0.0f, 4.0f, 0.0f},  {0.0f, 0.0f}},
            {{4.0f, 4.0f, 0.0f},  {1.0f, 0.0f}},
            {{4.0f, 0.0f, 0.0f},  {1.0f, 1.0f}},
            
            {{4.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
            {{0.0f, 4.0f, 0.0f}, {0.0f, 0.0f}}
        });
}

void GameClient::serverThreadFunction()
{
    try
    {
        if (server != nullptr) server->startServer(1);
    }
    catch(const NetworkError& err)
    {
        std::cerr << "Server thread error: " << err.what() << std::endl;
    }
}

void GameClient::update(Camera& camera, EventWrapper& events, const glm::vec2& camZoom)
{
    //**************************************************
    // Multiplayer Groups
    //**************************************************

    clientSocket->checkSocket(disconnectCallback, serverPlayers, serverChunks);
    serverPlayers.updatePlayers(serverChunks, timer, entities);
    
    //**************************************************
    // Chunks
    //**************************************************
    ChunkPos chunkPlayer = serverChunks.posToChunkPos(mainPlayer.position.x, -mainPlayer.position.y);
    serverChunks.loadPtr.x = chunkPlayer.x - (serverChunks.loadDistance.x / 2);
    serverChunks.loadPtr.y = chunkPlayer.y + (serverChunks.loadDistance.y / 2);
    
    serverChunks.update(camera);

    //**************************************************
    // Player
    //**************************************************
	mainPlayer.update(serverChunks, timer, entities);

	for (int i = 0; i < 4; ++i)
    {
		if (events.keyState[i])
        {
			mainPlayer.directPlayer(i, serverChunks, timer);
		}
	}
    
	// Jump (A)
	if (events.buttonState[4])
    {
		mainPlayer.directPlayer(0, serverChunks, timer);
	}
	// Down
	if (events.buttonState[0])
    {
		mainPlayer.directPlayer(2, serverChunks, timer);
	}
	// Right
	if (events.buttonState[1])
    {
		mainPlayer.directPlayer(1, serverChunks, timer);
	}
	// Left
	if (events.buttonState[2])
    {
		mainPlayer.directPlayer(3, serverChunks, timer);
	}
	// Up
	if (events.buttonState[3])
    {
		mainPlayer.directPlayer(0, serverChunks, timer);
	}
    
    handleCamera(camera);

    if (background) background->update(camera, time);

    mouseEvents(camera, events, camZoom);

    time.tick(timer, 40);
    fade.tick(timer, 40);

    if (playerPosUpdate.tick(timer, 120))
    {
        Api::sendPlayerPos(clientSocket->fd, mainPlayer.position.x, mainPlayer.position.y);
    }

    particleGroup.update(serverChunks, timer, particleModel);
}

void GameClient::render(Graphics& graphics, EventWrapper& events, const glm::vec2& camZoom) const
{
    if (background) background->render(graphics);

    // Draw chunks
    serverChunks.render(graphics, graphics.camera);

    particleGroup.draw(graphics, particleModel);

    // Draw other players
    serverPlayers.renderPlayers(graphics, graphics.camera);

    // Draw this player
    mainPlayer.render(graphics, graphics.camera);

    // Draw particles
    particleGroup.draw(graphics, particleModel);

    // Draw the selection box for where we are hovering
    drawSelection(graphics, getSelection(graphics.camera, events, camZoom));
}

void GameClient::mouseEvents(Camera& camera, EventWrapper& events, const glm::vec2& camZoom)
{
    constexpr size_t MAX_PARTICLES = 16;
    SelectInfo pos;
    /*if (!inv->showInventoryMenu)*/ pos = getSelection(camera, events, camZoom);
    int p1Fixed = pos.x, p2Fixed = pos.y;
    int withinX, withinY;

    
    // Get cursor over chunk
    if (pos.x < 0) {
        p1Fixed = pos.x - constants::chunkWidth + 1;
        withinX = (p1Fixed % constants::chunkWidth) + constants::chunkWidth - 1;
    } else {
        withinX = pos.x % constants::chunkWidth;
    }

    if (pos.y < 0) {
        p2Fixed = pos.y - constants::chunkHeight + 1;
        withinY = (p2Fixed % constants::chunkHeight) + constants::chunkHeight - 1;
    } else {
        withinY = pos.y % constants::chunkHeight;
    }

    int chunkX = p1Fixed / constants::chunkWidth;
    int chunkY = p2Fixed / constants::chunkHeight;
    Chunk* chunk = serverChunks.getChunkAt(chunkX, chunkY);
    if (chunk != nullptr)
    {
        switch(events.vmouse.down)
        {
        case 1:
        {
            const BlockInfo* block = chunk->destroyBlock(withinX, withinY);
            
            // Check if block found
            if (block != nullptr) {
                // Regenerate chunk mesh
                chunk->generateChunkMesh();
                // Send request to destroy block on server side
                Api::sendDestroyBlock(clientSocket->fd, chunkX, chunkY, withinX, withinY);
                // Generate particles
                if (constants::config.getInt(CONFIG_SHOW_PARTICLES)) {
                    for (size_t i = 0; i < MAX_PARTICLES; ++i)
                    {
                        particleGroup.addParticle({0,
                                block->textureId,
                                static_cast<double>(300 + (static_cast<float>(rand() % 100) / 100.0f)),
                                rand() % 2 == 1 ? -230.0f : 230.0f,
                                -180.0f,
                                static_cast<double>(pos.x) * constants::blockW +
                                (rand() % static_cast<int>(constants::blockW)),
                                static_cast<double>(pos.y) * constants::blockH +
                                (rand() % static_cast<int>(constants::blockH)),
                                4,
                                4});
                    }
                }
            }
        }
        break;
        case 3:
        {
            //Item& item = inv->getSelectedItem();
            //if (item.enabled)
            //BlockInfo& b_info = item.block;
            //item.addCount(-1);
            
            if (chunk->placeBlock(0, withinX, withinY)) {
                chunk->generateChunkMesh();
                Api::sendPlaceBlock(clientSocket->fd, 0, chunkX, chunkY, withinX, withinY);
            }
        }
        break;
        default:
			break;
		}
    }
}

SelectInfo GameClient::getSelection(Camera& camera, EventWrapper& events, const glm::vec2& camZoom) const
{
    const int& mouseX = events.vmouse.x;
    const int& mouseY = events.vmouse.y;
    
    const int& selX = std::floor((mouseX + camera.position.x) / constants::blockW);
    const int& selY = std::floor((mouseY + camera.position.y) / constants::blockH);

    return {selX, selY};
}

void GameClient::drawSelection(const Graphics& graphics, const SelectInfo pos) const
{
    SDL_Rect selection{static_cast<int>(pos.x * constants::blockW + graphics.camera.position.x),
        static_cast<int>(pos.y * constants::blockH + graphics.camera.position.x),
        constants::blockW, constants::blockH};

    int fadeAmount = std::abs(std::sin(fade.frame / 20.0f)) * 30.0f + 50.0f;

    //SDL_SetRenderDrawColor(graphics, 255, 255, 255, fadeAmount);
    //SDL_RenderFillRect(graphics, &selection);
}

void GameClient::handleCamera(Camera& camera)
{
	double x = -std::floor(mainPlayer.position.x) + (winSize.w / 2) - mainPlayer.size.x/2;
	double y = -std::floor(mainPlayer.position.y) + (winSize.h / 2) - mainPlayer.size.y/2;
	
	static double moveX = x;
	static double moveY = y;

	float amount = 15.0f;
	moveX += (x - moveX) * amount * timer.deltaTime;
	moveY += (y - moveY) * amount * timer.deltaTime;

    // Rounded since screen is in pixels and makes things look less blurry
    camera.position.x = -static_cast<int>(moveX);
    camera.position.y = -static_cast<int>(moveY);
    camera.center.x = -static_cast<int>(moveX);
    camera.center.y = -static_cast<int>(moveY);
}
