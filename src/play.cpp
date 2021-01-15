#include "play.hpp"

Play::Play(SDL_Renderer* renderer, TextureHandler& textures, EventWrapper& events, WinSize& winSize)
	: winSize{winSize},
      renderer{renderer},
      textures{textures},
      events{events},
      chunks{0}, 
	  camera{&winSize},
      player{},
      entities{},
      fade{60},
      particles{},
      time{8600, 28500, 8600, 22000, 1700, 1700},
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
	
	// Update all chunks
	chunks.update_all_viewport(camera);
	chunks.check_area(player.obj.x, structures);
	
	inv->update();

	// Update all entities
	for (Entity*& entity: entities)
    {
		if (!entity->shouldCull(camera))
        {
			entity->update(chunks);
		}
	}
	
	// Update player
	player.update(chunks, entities);

	for (int i = 0; i < 4; ++i)
    {
		if (events.keyState[i])
        {
			player.directPlayer(i, chunks);
		}
	}

	// Create entity
	if (events.keyState[16] || events.buttonState[9])
    {
		entities.push_back(new TestEntity(player.obj.x, player.obj.y-30));
	}

	// Jump (A)
	if (events.buttonState[4])
    {
		player.directPlayer(0, chunks);
	}

	// Down
	if (events.buttonState[0])
    {
		player.directPlayer(2, chunks);
	}

	// Right
	if (events.buttonState[1])
    {
		player.directPlayer(1, chunks);
	}

	// Left
	if (events.buttonState[2])
    {
		player.directPlayer(3, chunks);
	}

	// Up
	if (events.buttonState[3])
    {
		player.directPlayer(0, chunks);
	}


	// Update animation(s)
	fade.tick();
	
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
		
		for (size_t i = 0; i < data.size(); ++i)
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
		}
	}
	
	// Particles
	deadParticles();

	if (constants::config.getInt(CONFIG_SHOW_PARTICLES))
    {
		for (auto& particle: particles)
        {
			particle.update(chunks);
		}
	}

	handleCamera();

	// Update background (after we handle camera or things get a tiny bit off sync)
	background->update(camera, time);

    time.tick();
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
	
	chunks.render_all_viewport(renderer, textures, camera);
	
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
	int p1, p2;
	if (!inv->showInventoryMenu) drawSelection(&p1, &p2);
	
	// Get cursor over chunk
	Chunk* chunk = chunks.get_chunk_at(p1*constants::blockW, true);
	if (chunk != nullptr)
    {
		// Do some math to get the chunk position
		int chunk_pos = std::abs(p1-(chunk->get_slot()*constants::chunkWidth));

		switch(events.vmouse.down)
        {
		case 1:
        {
            const BlockInfo* block = chunk->destroy_block(chunk_pos, p2, inv.get());
            
            // Check if block found
            if (block != nullptr) {
                // Generate particles
                if (constants::config.getInt(CONFIG_SHOW_PARTICLES)) {
                    GravityParticle temp{block->textureId,50, rand() % 2 == 1 ? -2.0 : 2.0, -3.0,
                        p1*constants::blockW+(constants::blockW/2), p2*constants::blockH, 8, 6};
                    particles.push_back(temp);
                }
            }
        }
        break;
		case 3:
        {
            Item& item = inv->getSelectedItem();
            if (item.enabled) {
                BlockInfo& b_info = item.block;
                if (chunk->place_block(b_info.id, chunk_pos, p2)) {
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
	int b_w = static_cast<int>(constants::blockW);
	int b_h = static_cast<int>(constants::blockH);

	const int sel_x = floor((events.vmouse.x - camera.x) / b_w) * b_w;
	const int sel_y = floor((events.vmouse.y - camera.y) / b_h) * b_h;

    SDL_Rect selection{sel_x + static_cast<int>(camera.x), sel_y + static_cast<int>(camera.y), b_w, b_h};

	int fade_amount = std::abs(std::sin(static_cast<double>(fade.frame) / 20)) * 30 + 50;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, fade_amount);
    SDL_RenderFillRect(renderer, &selection);

	if (p1 != nullptr) *p1 = sel_x / b_w;
	if (p2 != nullptr) *p2 = sel_y / b_h;
}

// Sets camera to player position
void Play::handleCamera()
{
	double x = -std::floor(player.obj.x) + (winSize.w / 2) - player.obj.w/2;
	double y = -std::floor(player.obj.y) + (winSize.h/2) - player.obj.h/2;
	
	static double moveX = x;
	static double moveY = y;

	float amount = 0.25;
	moveX += (x - moveX) * amount;
	moveY += (y - moveY) * amount;
	
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
