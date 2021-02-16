#include "gameclient.hpp"

GameClient::GameClient(std::string address, uint16_t port, Timer& timer, WinSize& winSize)
    : winSize{winSize},
      serverChunks{},
      camera{&winSize},
      mainPlayer{},
      entities{},
      time{8600, 28500, 8600, 22000, 1700, 1700},
      timer{timer}
{
    camera.x = 150;
    camera.y = 450;
}

GameClient::~GameClient()
{}

void GameClient::update(EventWrapper& events)
{
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

	// Create entity
	// if (events.keyState[16] || events.buttonState[9])
    // {
	// 	entities.push_back(new TestEntity(mainPlayer.position.x, mainPlayer.position.y-30));
	// }

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

    handleCamera();
}

void GameClient::render(SDL_Renderer* renderer, TextureHandler& textures)
{
    serverChunks.render(renderer, textures, camera);

    mainPlayer.render(renderer, textures, camera);
}

void GameClient::handleCamera()
{
	double x = -std::floor(mainPlayer.position.x) + (winSize.w / 2) - mainPlayer.size.w/2;
	double y = -std::floor(mainPlayer.position.y) + (winSize.h / 2) - mainPlayer.size.h/2;
	
	static double moveX = x;
	static double moveY = y;

	float amount = 15.0f;
	moveX += (x - moveX) * amount * timer.deltaTime;
	moveY += (y - moveY) * amount * timer.deltaTime;
	
    camera.x = moveX;
    camera.y = moveY;
}
