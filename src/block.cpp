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

unsigned char* Block::serialize()
{
    unsigned char id[2];
    unsigned char positionX[4];
    unsigned char positionY[4];

    // Id serialize
    uint16_t valueid = blockinfo->id;
    // fill char array with segmant of binary per valueid
    id[0] = (valueid>>8) & 0xff; // i use 0xff because it makes me look cooler
    id[1] = valueid & 0xff;


    // Serialize both positionX and positionY
    unsigned char* charWork = positionX;
    int32_t posWork = position.x / constants::blockW;
    for (int i = 0; i <= 1; ++i)
    {
        charWork[0] = (posWork>>24) & 0xff;
        charWork[1] = (posWork>>16) & 0xff;
        charWork[2] = (posWork>>8) & 0xff;
        charWork[3] = posWork & 0xff;
        
        // flip bit at position
        charWork[0] ^= (posWork<0)<<7;

        
        charWork = positionY;
        posWork = position.y / constants::blockH;
    }    
}

void Block::deserialize(const std::string& str)
{

}
