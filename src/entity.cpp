#include "entity.hpp"

Entity::Entity(int textureId, double x, double y, double width, double height)
	: GameObject{textureId, x, y, width, height},
      velX{0},
      velY{0},
      velXSpeed{1.8},
      onGround{false},
      lastPos{-1}
{}

Entity::~Entity()
{}

void Entity::update(ChunkGroup& chunks)
{
	// Optional; You can use your own physics function
	updateBasicPhysics(chunks);
}

void Entity::collisionLeft() {}
void Entity::collisionRight() {}
void Entity::collisionBottom() {}
void Entity::collisionTop() {}

CollisionInfo Entity::checkBlockCollision(ChunkGroup& chunks)
{
	std::vector<Chunk*>& chunkgroup = chunks.get_viewport_chunks();

	Chunk* c = chunks.get_chunk_at(obj.x, true);
	Chunk* c_front = chunks.get_chunk_at(obj.x+obj.w, true);

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
				chunks.get_chunk_y(obj.y),
				chunks.get_chunk_y(obj.y+obj.h)
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

void Entity::updateBasicPhysics(ChunkGroup& chunks) {
    constexpr int cap = 36;
    
	// Update draw position
	src.x = 0;
	src.y = 0;
    src.w = obj.w;
	src.h = obj.h;
	
	if (onGround) velX *= 0.78;

	// Reset back
	if (velX > 6) velX = 6;
	if (velX < -6) velX = -6;
		
	obj.x += velX;

	CollisionInfo infoX = checkBlockCollision(chunks);

	// Check X velocity
	if (infoX == true) {
		if (velX == 0) {
			// If entity happens to get stuck in the wall then push them out
			if (lastPos == 1) {
				obj.x -= 5;
			} else if (lastPos == 3) {
				obj.x += 5;
			} else {
				obj.x += 5;
			}
				
		}
		if (infoX.left != -1) {
			obj.x -= infoX.left;
		}
		if (infoX.right != -1) {
			obj.x += infoX.right;
		}
		velX = 0;
		onGround = true;
	}
		
	velY += .5;
	obj.y += velY;

	// Cap +Y velocity
	if (velY > cap) {
		velY = cap;
	}

	CollisionInfo infoY = checkBlockCollision(chunks);
		
	// Check Y velocity
	if (infoY == true) {
		if (infoY.bottom != -1) {
			obj.y += infoY.bottom;
		}
		if (infoY.top != -1) {
			obj.y -= infoY.top;
		}
		velY = 0;
		onGround = true;
	} else {
		onGround = false;
	}
}
