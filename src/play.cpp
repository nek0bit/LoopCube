#include "play.hpp"

Play::Play(SDL_Renderer* renderer, TextureHandler& textures, EventWrapper& events, Timer& timer, WinSize& winSize)
	: winSize{winSize},
      renderer{renderer},
      textures{textures},
      events{events},
      generator{std::make_shared<ChunkGen>(ChunkGen{1})},
      chunks{generator}, 
	  camera{&winSize},
      player{},
      entities{},
      fade{60},
      particles{},
      time{8600, 28500, 8600, 22000, 1700, 1700},
      timer{timer},
	  background{nullptr}
{
    background = std::shared_ptr<Background>(new BackgroundOverworld());
    camera.x = 0;
    camera.y = 0;

	inv = std::unique_ptr<Inventory>(new Inventory(renderer, textures, events, winSize));
}

Play::~Play() {}

void Play::update()
{
	mouseEvents();
	
	//**************************************************
    // Chunks
    //**************************************************
    ChunkPos chunkPlayer = chunks.posToChunkPos(player.position.x, -player.position.y);
    chunks.loadPtr.x = chunkPlayer.x - (chunks.loadDistance.x / 2);
    chunks.loadPtr.y = chunkPlayer.y + (chunks.loadDistance.y / 2);

    
    chunks.update(camera);
	
	inv->update(timer);
    
	//**************************************************
    // Entities
    //**************************************************
	for (Entity*& entity: entities)
    {
		if (!entity->shouldCull(camera))
        {
			entity->update(chunks, timer);
		}
	}
	

    //**************************************************
    // Player
    //**************************************************
	player.update(chunks, timer, entities);

	for (int i = 0; i < 4; ++i)
    {
		if (events.keyState[i])
        {
			player.directPlayer(i, chunks, timer);
		}
	}

	// Create entity
	if (events.keyState[16] || events.buttonState[9])
    {
		entities.push_back(new TestEntity(player.position.x, player.position.y-30));
	}

	// Jump (A)
	if (events.buttonState[4])
    {
		player.directPlayer(0, chunks, timer);
	}

	// Down
	if (events.buttonState[0])
    {
		player.directPlayer(2, chunks, timer);
	}

	// Right
	if (events.buttonState[1])
    {
		player.directPlayer(1, chunks, timer);
	}

	// Left
	if (events.buttonState[2])
    {
		player.directPlayer(3, chunks, timer);
	}

	// Up
	if (events.buttonState[3])
    {
		player.directPlayer(0, chunks, timer);
	}


	// Update animation(s)
	fade.tick(timer, 50);
	
	// Structures Queue Handling
	for (size_t k = 0; k < structures.size(); ++k)
    {
		auto& data = structures[k]->get_data();
		
		// Make sure we delete old structures to prevent memleaks
		if (data.size() == 0)
        {
			delete structures[k];
			structures.erase(structures.begin() + k);
			continue;
		}
		
		/*for (size_t i = 0; i < data.size(); ++i)
        {
			// See if the chunk actually exists
			auto c1 = chunks.get_chunk_at((structures[k]->get_x()+data[i].x)*constants::blockW, false);
			
			if (c1 != nullptr)
            {
				int chunk_pos = std::abs(((structures[k]->get_x()+data[i].x))-(c1->get_slot()*constants::chunkWidth));

				// Place block in proper chunk
				c1->place_block(data[i].id, chunk_pos, structures[k]->get_y() + data[i].y);
				
				// Erase the data now that we are done using it
				data.erase(data.begin() + i);
			}
            }*/
	}
	
	// Particles
	deadParticles();

	if (constants::config.getInt(CONFIG_SHOW_PARTICLES))
    {
		for (auto& particle: particles)
        {
			particle.update(chunks, timer);
		}
	}

	handleCamera();

	// Update background (after we handle camera or things get a tiny bit off sync)
	background->update(camera, time);

    time.tick(timer, 40);
}

void Play::render() {
	// Render background elements
	background->render(renderer, textures);
	
	if (constants::config.getInt(CONFIG_SHOW_PARTICLES))
    {
		for (auto& particle: particles)
        {
			particle.render(renderer, textures, camera);
		}
	}
	
	chunks.render(renderer, textures, camera);
	
	player.render(renderer, textures, camera);

	for (Entity*& entity: entities)
    {
		entity->render(renderer, textures, camera);
	}

	if (!inv->showInventoryMenu) drawSelection(nullptr, nullptr);

	inv->drawHotbar();

	inv->drawInventoryMenu();
}

void Play::mouseEvents() {
    constexpr size_t MAX_PARTICLES = 10;
	int p1, p2;
	if (!inv->showInventoryMenu) drawSelection(&p1, &p2);
    int p1Fixed = p1, p2Fixed = p2;
    int withinX, withinY;

    
	// Get cursor over chunk
    if (p1 < 0) {
        p1Fixed = p1 - constants::chunkWidth + 1;
        withinX = (p1Fixed % constants::chunkWidth) + constants::chunkWidth - 1;
    } else {
        withinX = p1 % constants::chunkWidth;
    }

    if (p2 < 0) {
        p2Fixed = p2 - constants::chunkHeight + 1;
        withinY = (p2Fixed % constants::chunkHeight) + constants::chunkHeight - 1;
    } else {
        withinY = p2 % constants::chunkHeight;
    }
    
    std::shared_ptr<Chunk> chunk = chunks.getChunkAt(p1Fixed / constants::chunkWidth, p2Fixed / constants::chunkHeight);
	if (chunk != nullptr)
    {
		switch(events.vmouse.down)
        {
		case 1:
        {
            
            const BlockInfo* block = chunk->destroyBlock(withinX, withinY, *inv);
            
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
                            p1 * constants::blockW + (rand() % static_cast<int>(constants::blockW)),
                            p2 * constants::blockH + (rand() % static_cast<int>(constants::blockH)),
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
            Item& item = inv->getSelectedItem();
            if (item.enabled) {
                BlockInfo& b_info = item.block;
                if (chunk->placeBlock(b_info.id, 0, p2)) {
                    item.addCount(-1);
                }
            }
        }
        break;
		default:
			break;
		}
    }
}

// Draw a selection box and set p1 and p2 to the position
// Perhaps we should rename this to get_selection and seperate rendering functions?
void Play::drawSelection(int* p1, int* p2)
{
    float b_w = static_cast<float>(constants::blockW);
    float b_h = static_cast<float>(constants::blockH);

	const float sel_x = floor((events.vmouse.x - camera.x) / b_w) * b_w;
	const float sel_y = floor((events.vmouse.y - camera.y) / b_h) * b_h;

    SDL_Rect selection{static_cast<int>(sel_x + camera.x), static_cast<int>(sel_y + camera.y), b_w, b_h};

	int fade_amount = std::abs(std::sin(fade.frame / 20.0f)) * 30.0f + 50.0f;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, fade_amount);
    SDL_RenderFillRect(renderer, &selection);

	if (p1 != nullptr) *p1 = sel_x / b_w;
	if (p2 != nullptr) *p2 = sel_y / b_h;
}

// Sets camera to player position
void Play::handleCamera()
{
	double x = -std::floor(player.position.x) + (winSize.w / 2) - player.size.w/2;
	double y = -std::floor(player.position.y) + (winSize.h / 2) - player.size.h/2;
	
	static double moveX = x;
	static double moveY = y;

	float amount = 15.0f;
	moveX += (x - moveX) * amount * timer.deltaTime;
	moveY += (y - moveY) * amount * timer.deltaTime;
	
    camera.x = moveX;
    camera.y = moveY;
}

void Play::deadParticles()
{
	for (size_t i = 0; i < particles.size(); ++i)
    {
		if (particles[i].isDead()) particles.erase(particles.begin() + i);
	}
}
