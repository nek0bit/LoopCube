#include "gameclient.hpp"

GameClient::GameClient(std::string address, uint16_t port, Timer& timer, WinSize& winSize)
    : winSize{winSize},
      serverChunks{},
      camera{&winSize},
      mainPlayer{},
      entities{},
      fade{60},
      particles{},
      time{8600, 28500, 8600, 22000, 1700, 1700},
      timer{timer}
{
    camera.x = 150;
    camera.y = 450;
}

GameClient::~GameClient()
{
}

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

    mouseEvents(events);

    // update particles
    if (constants::config.getInt(CONFIG_SHOW_PARTICLES))
    {
        for (auto& particle: particles)
        {
            particle.update(serverChunks, timer);
        }
    }

    time.tick(timer, 40);
    fade.tick(timer, 40);

    deadParticles();
}

void GameClient::render(SDL_Renderer* renderer, TextureHandler& textures, EventWrapper& events)
{
    serverChunks.render(renderer, textures, camera);

    mainPlayer.render(renderer, textures, camera);

    // Draw particles
    if (constants::config.getInt(CONFIG_SHOW_PARTICLES))
    {
        for (auto& particle: particles)
        {
            particle.render(renderer, textures, camera);
        }
    }

    drawSelection(renderer, getSelection(events));
}

void GameClient::deadParticles()
{
    for (size_t i = 0; i < particles.size(); ++i)
    {
        if (particles[i].isDead()) particles.erase(particles.begin() + i);
    }
}

void GameClient::mouseEvents(EventWrapper& events)
{
    constexpr size_t MAX_PARTICLES = 4;
    SelectInfo pos;
    /*if (!inv->showInventoryMenu)*/ pos = getSelection(events);
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
    
    std::shared_ptr<Chunk> chunk = serverChunks.getChunkAt(p1Fixed / constants::chunkWidth, p2Fixed / constants::chunkHeight);
    if (chunk != nullptr)
    {
        switch(events.vmouse.down)
        {
        case 1:
        {
            const BlockInfo* block = chunk->destroyBlock(withinX, withinY);
            
            // Check if block found
            if (block != nullptr) {
                // Generate particles
                if (constants::config.getInt(CONFIG_SHOW_PARTICLES)) {
                    for (size_t i = 0; i < MAX_PARTICLES; ++i)
                    {
                        GravityParticle temp{block->textureId,
                            .3 + (static_cast<float>(rand() % 100) / 100.0f),
                            rand() % 2 == 1 ? -230.0f : 230.0f,
                            -180.0f,
                            pos.x * constants::blockW + (rand() % static_cast<int>(constants::blockW)),
                            pos.y * constants::blockH + (rand() % static_cast<int>(constants::blockH)),
                            8,
                            6};
                        particles.push_back(temp);
                    }
                }
            }
        }
        break;
        case 3:
        {
            //Item& item = inv->getSelectedItem();
            //if (item.enabled) {
            //BlockInfo& b_info = item.block;
            if (chunk->placeBlock(0, withinX, withinY)) {
                //item.addCount(-1);
            }
            //}
        }
        break;
        default:
			break;
		}
    }
}

SelectInfo GameClient::getSelection(EventWrapper& events)
{
    const int selX = floor((events.vmouse.x - camera.x) / constants::blockW);
    const int selY = floor((events.vmouse.y - camera.y) / constants::blockH);

    return {selX, selY};
}

void GameClient::drawSelection(SDL_Renderer* renderer, const SelectInfo pos)
{
    SDL_Rect selection{static_cast<int>(pos.x * constants::blockW + camera.x),
        static_cast<int>(pos.y * constants::blockH + camera.y),
        constants::blockW, constants::blockH};

    int fadeAmount = std::abs(std::sin(fade.frame / 20.0f)) * 30.0f + 50.0f;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, fadeAmount);
    SDL_RenderFillRect(renderer, &selection);
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
