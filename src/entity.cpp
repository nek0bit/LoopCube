#include "entity.hpp"

Entity::Entity(int modelId, int textureId, glm::vec3 position, glm::vec2 size)
	: GameObject{modelId, textureId, position, size},
      velX{0},
      velY{0},
      displayPosition{0, 0},
      velXSpeed{9000},
      onGround{false},
      lastPos{-1}
{
	// Update draw position
	src.x = 0;
	src.y = 0;
    src.w = size.x;
	src.h = size.y;
}

Entity::~Entity()
{}

void Entity::update() {}

void Entity::update(ChunkGroup& chunks, const Timer& timer)
{
	// Optional; You can use your own physics function
	updateBasicPhysics(chunks, timer);
}

#ifndef __HEADLESS
void Entity::render(const Graphics& renderer, const Camera& camera) const
{
    const glm::vec2 val = displayPosition + static_cast<glm::vec2>(camera.position);
	SDL_Rect dest{static_cast<int>(val.x),
        static_cast<int>(val.y),
        static_cast<int>(size.x),
        static_cast<int>(size.y)};
    //SDL_RenderCopy(renderer, textures.getTexture(textureId)->texture, &src, &dest);
}
#endif

void Entity::collisionLeft() {}
void Entity::collisionRight() {}
void Entity::collisionBottom() {}
void Entity::collisionTop() {}

CollisionInfo Entity::checkBlockCollision(ChunkGroup& chunks)
{
    // TODO optimize entity position to work within blocks instead of chunks
    // It's 100% easy and possible and should optimize a lot

    std::vector<Chunk*> inChunks = chunks.isWithinChunks(position, size);

    for (auto& chunk: inChunks)
    {
        if (chunk == nullptr) continue;
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

void Entity::updateBasicPhysics(ChunkGroup& chunks, const Timer& timer) {
    const int cap = 2000.0f;
    constexpr float friction = 20.0f;

    // Return if not within valid chunks
    std::vector<Chunk*> inChunks = chunks.isWithinChunks(position, size);
    for (auto& chunk: inChunks)
    {
        if (chunk == nullptr) return;
    }
	
    velX *= 1 / (1 + (timer.deltaTime * friction));
		
    position.x += velX * timer.deltaTime;

    CollisionInfo infoX = checkBlockCollision(chunks);

    // Check X velocity
    if (infoX == true) {
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
        if (infoY.top != -1) {
            position.y -= infoY.top;
        }
        if (infoY.bottom != -1) {
            position.y += infoY.bottom;
        }
        velY = 0;
        onGround = true;
    } else {
        onGround = false;
    }

    displayPosition = position;
    
}

void Entity::dummyInterpolate(const Timer& timer)
{
    displayPosition.x = Generic::lerp(displayPosition.x, position.x, .25);
    displayPosition.y = Generic::lerp(displayPosition.y, position.y, .25);
}
