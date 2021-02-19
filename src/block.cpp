#include "block.hpp"

Block::Block()
    : blockinfo{nullptr},
      typeX{0}
{}

Block::Block(int id, int x, int y, unsigned int typeX)
	: GameObject{0, static_cast<double>(x * constants::blockW),
    static_cast<double>(y * constants::blockH),
    static_cast<double>(constants::blockW),
    static_cast<double>(constants::blockH)},
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

std::vector<unsigned char> Block::serialize() const
{
    // TODO determine the minimum size we can fit it into (set bits/8)
    // Example: block at position 1 can fit into uint8 and 256 into uint16
    std::vector<unsigned char> fullRes;
    constexpr uint8_t BYTE_SIZE = sizeof(unsigned char) * 8;
    const uint8_t idLen = sizeof(blockinfo->id); // For now, we'll kind of use fixed lengths
    const uint8_t xLen = sizeof(uint32_t);
    const uint8_t yLen = sizeof(uint32_t);
    uint16_t maxLen = idLen+1;

    fullRes.push_back(idLen);
    
    // Store id
    for (int in = 0, maxIdLen = 0; maxIdLen < idLen; ++maxIdLen)
    {
        fullRes.push_back((blockinfo->id >> in) & 0xff);
        in += BYTE_SIZE;
    }

    fullRes.push_back(xLen);

    // Store X
    int32_t convertX_s = position.x / constants::blockW;
    uint32_t convertX = std::abs(convertX_s);
    for (int in = 0, maxXLen = 0; maxXLen < xLen; ++maxXLen)
    {
        fullRes.push_back((convertX >> in) & 0xff);
        in += BYTE_SIZE;
    }
    fullRes[maxLen+xLen] ^= (convertX_s<0)<<7;
    maxLen += xLen+1;
    
    fullRes.push_back(yLen);
    
    // Store Y
    int32_t convertY_s = position.y / constants::blockH;
    uint32_t convertY = std::abs(convertY_s);
    for (int in = 0, maxYLen = 0; maxYLen < yLen; ++maxYLen)
    {
        fullRes.push_back((convertY >> in) & 0xff);
        in += BYTE_SIZE;
    }
    fullRes[maxLen+yLen] ^= (convertY_s<0)<<7;
    maxLen += yLen+1;

    // Push back stop byte
    fullRes.push_back(255);
    
    return fullRes;
}

void Block::deserialize(const std::array<unsigned char, 10>& value)
{
    uint32_t res = 0;
    
}
