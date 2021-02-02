#include "block.hpp"

Block::Block()
    : blockinfo{nullptr},
      typeX{0},
      typeY{0}
{}

Block::Block(int id, int x, int y, unsigned int typeX, unsigned int typeY)
	: GameObject{0, static_cast<double>(x * constants::blockW),
    static_cast<double>(y * constants::blockH),
    constants::blockW,
    constants::blockH},
      blockinfo{nullptr},
      typeX{typeX},
      typeY{typeY}
{
    // TODO don't search
	for (auto &i: constants::blockInfo) {
		if (i.id == id) {
			this->textureId = i.textureId;
			this->blockinfo = &i;
		}
	}

    
	src.h = constants::blockImgSize;
	src.w = constants::blockImgSize;
    updateSrc();
}

Block::~Block()
{}

void Block::updateSrc()
{
    src.x = typeX * constants::blockImgSize;
	src.y = typeY * constants::blockImgSize;
}

void Block::update()
{}

void Block::renderShadow(SDL_Renderer* renderer, Camera& camera) const
{
    const Vec2 val = getPos(camera);
	SDL_Rect shadow{static_cast<int>(val.x + 5), static_cast<int>(val.y + 5),
		static_cast<int>(size.w), static_cast<int>(size.h)};
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 60);
    SDL_RenderFillRect(renderer, &shadow);
}
