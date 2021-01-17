#include "entity.hpp"

Entity::Entity(int textureId, double x, double y, double width, double height)
	: GameObject{textureId, x, y, width, height},
      velX{0},
      velY{0},
      velXSpeed{.009},
      onGround{false},
      lastPos{-1}
{}

Entity::~Entity()
{}

void Entity::update(ChunkGroup& chunks, Timer& timer)
{
	// Optional; You can use your own physics function
	updateBasicPhysics(chunks, timer);
}

void Entity::collisionLeft() {}
void Entity::collisionRight() {}
void Entity::collisionBottom() {}
void Entity::collisionTop() {}

CollisionInfo Entity::checkBlockCollision(ChunkGroup& chunks)
{
	std::vector<Chunk*>& chunkgroup = chunks.get_viewport_chunks();

	Chunk* c = chunks.get_chunk_at(position.x, true);
	Chunk* c_front = chunks.get_chunk_at(position.x+size.w, true);

	if (c == nullptr || c_front == nullptr) {
		return CollisionInfo{};
	}

	// Loop through all chunks
	for (auto *&chunk_it: chunkgroup) {
		// See if player is within this chunk, if so, move on and handle collision
		// If not, it's pointless to check the chunk, continue
		if (chunk_it->get_slot() == c->get_slot() ||
			chunk_it->get_slot() == c_front->get_slot()) {

			// Store chunk as reference for further usage
			std::vector<std::vector<Block>>& chunk = chunk_it->get_chunk();

			std::array<int, 2> player_check = {
				chunks.get_chunk_y(position.y),
				chunks.get_chunk_y(position.y+size.h)
			};
			
			
			for (auto &i: player_check) {
				if (i == -1) {
					continue;
				}
				for (auto &block: chunk[i]) {
					auto blockinfo = block.blockinfo;
					CollisionInfo info = isColliding(block);
					while (info == true && blockinfo->noCollision != true) {
						return info;
					}
				}
			}
		}
	}
	return CollisionInfo{};
}

void Entity::updateBasicPhysics(ChunkGroup& chunks, Timer& timer) {
    const int cap = 2;
    constexpr float friction = 20.0f;
    
	// Update draw position
	src.x = 0;
	src.y = 0;
    src.w = size.w;
	src.h = size.h;
	
	velX *= 1 / (1 + (timer.deltaTime.s * friction));
		
    position.x += velX * timer.deltaTime.ms;

	CollisionInfo infoX = checkBlockCollision(chunks);

	// Check X velocity
	if (infoX == true) {
		if (velX == 0) {
			// If entity happens to get stuck in the wall then push them out
			if (lastPos == 1) {
			    position.x -= 5;
			} else if (lastPos == 3) {
			    position.x += 5;
			} else {
			    position.x += 5;
			}
				
		}
		if (infoX.left != -1) {
		    position.x -= infoX.left;
		}
		if (infoX.right != -1) {
		    position.x += infoX.right;
		}
		velX = 0;
		onGround = true;
	}
		
	velY += .002 * timer.deltaTime.ms;
    position.y += velY * timer.deltaTime.ms;

	// Cap +Y velocity
	if (velY > cap) {
		velY = cap;
	}

	CollisionInfo infoY = checkBlockCollision(chunks);
		
	// Check Y velocity
	if (infoY == true) {
		if (infoY.bottom != -1) {
		    position.y += infoY.bottom;
		}
		if (infoY.top != -1) {
		    position.y -= infoY.top;
		}
		velY = 0;
		onGround = true;
	} else {
		onGround = false;
	}
}
