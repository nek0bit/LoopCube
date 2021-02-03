#include "entity.hpp"

Entity::Entity(int textureId, double x, double y, double width, double height)
	: GameObject{textureId, x, y, width, height},
      velX{0},
      velY{0},
      velXSpeed{9000},
      onGround{false},
      lastPos{-1}
{
	// Update draw position
	src.x = 0;
	src.y = 0;
    src.w = size.w;
	src.h = size.h;
}

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
    // TODO URGENT optimize entity position to work within blocks instead of chunks
    // It's 100% easy and possible and should optimize a lot

    std::vector<std::shared_ptr<Chunk>> inChunks = chunks.isWithinChunks(position, size);

    for (auto& chunk: inChunks)
    {
        t_blockCollection& data = chunk->data;
        for (auto& block: data) {
            if (block == nullptr) continue;
            auto blockinfo = block->blockinfo;
            CollisionInfo info = isColliding(*block);
            while (info == true && blockinfo->noCollision != true) {
                return info;
            }
        }
    }
    
	return CollisionInfo{};
}

void Entity::updateBasicPhysics(ChunkGroup& chunks, Timer& timer) {
    const int cap = 2000.0f;
    constexpr float friction = 20.0f;
    
	
	velX *= 1 / (1 + (timer.deltaTime * friction));
		
    position.x += velX * timer.deltaTime;

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
		
	velY += 2000.0f * timer.deltaTime;
    position.y += velY * timer.deltaTime;

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
