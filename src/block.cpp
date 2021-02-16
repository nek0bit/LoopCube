#include "block.hpp"

Block::Block()
    : blockinfo{nullptr},
      typeX{0}
{}

Block::Block(int id, int x, int y, unsigned int typeX)
	: GameObject{0, static_cast<double>(x * constants::blockW),
    static_cast<double>(y * constants::blockH),
    constants::blockW,
    constants::blockH},
      blockinfo{nullptr},
      typeX{typeX}
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
#ifndef __HEADLESS
    updateSrc();
#endif
}

Block::~Block()
{}

void Block::update()
{}

#ifndef __HEADLESS
void Block::updateSrc()
{
    src.x = typeX * constants::blockImgSize;
}

void Block::renderShadow(SDL_Renderer* renderer, Camera& camera) const
{
    const Vec2 val = getPos(camera);
	SDL_Rect shadow{static_cast<int>(val.x + 5), static_cast<int>(val.y + 5),
		static_cast<int>(size.w), static_cast<int>(size.h)};
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 60);
    SDL_RenderFillRect(renderer, &shadow);
}
#endif

std::array<unsigned char, 10> Block::serialize() const
{
    constexpr uint8_t idSize = 2;
    constexpr uint8_t valSize = 4;
    std::array<unsigned char, 10> fullRes;

    // Id serialize
    uint16_t valueid = blockinfo->id;
    // fill char array with segmant of binary per valueid
    fullRes[0] = (valueid>>8) & 0xff; // i use 0xff because it makes me look cooler
    fullRes[1] = valueid & 0xff;


    // Serialize both positionX and positionY
    int32_t posWork = position.x / constants::blockW;
    
    for (int i = 0, j = idSize; i <= 1; ++i)
    {
        fullRes[j] = (posWork>>24) & 0xff;
        fullRes[j+1] = (posWork>>16) & 0xff;
        fullRes[j+2] = (posWork>>8) & 0xff;
        fullRes[j+3] = posWork & 0xff;
        
        // flip bit at position
        fullRes[j] ^= (posWork<0)<<7;

        
        posWork = position.y / constants::blockH;
        j += valSize;
    }
    
    return fullRes;
}

void Block::deserialize(const std::array<unsigned char, 10>& value)
{

}
