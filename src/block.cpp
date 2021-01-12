#include "block.hpp"

Block::Block() {}

Block::Block(int id, int x, int y)
	: GameObject{0, static_cast<double>(x * constants::blockW),
    static_cast<double>(y * constants::blockH),
    constants::blockW,
    constants::blockH}
{
    // Get block id
	for (auto &i: constants::blockInfo) {
		if (i.id == id) {
			this->texture_id = i.textureId;
			this->blockinfo = &i;
		}
	}
}

Block::~Block()
{}

void Block::update() {
	src.h = constants::blockImgSize;
	src.w = constants::blockImgSize;
	src.x = 0;
	src.y = 0;
}

void Block::renderShadow(SDL_Renderer* renderer, Camera& camera) const {
	SDL_Rect shadow{static_cast<int>(getX(camera) + 5), static_cast<int>(getY(camera) + 5),
		static_cast<int>(obj.w), static_cast<int>(obj.h)};
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 60);
    SDL_RenderFillRect(renderer, &shadow);
}
