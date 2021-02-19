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
    setBlockId(id);
    
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

void Block::setBlockId(const uint32_t id)
{
    try
    {
        blockinfo = &constants::blockInfo.at(id);
        textureId = this->blockinfo->id;
    } catch (const std::out_of_range& err) {}
}


std::vector<unsigned char> Block::serialize() const
{
    // TODO determine the minimum size we can fit it into (set bits/8)
    // Example: block at position 1 can fit into uint8 and 256 into uint16
    std::vector<unsigned char> fullRes;
    constexpr uint8_t BYTE_SIZE = sizeof(unsigned char) * 8;
    uint16_t maxLen = 0;
    const uint8_t idLen = sizeof(blockinfo->id); // For now, we'll kind of use fixed lengths
    //const uint8_t xLen = sizeof(uint32_t);
    //const uint8_t yLen = sizeof(uint32_t);

    // Push back the id so we can know its length for deserialization!
    fullRes.push_back(idLen);
    
    // Store id values
    for (int in = 0, maxIdLen = 0; maxIdLen < idLen; ++maxIdLen)
    {
        fullRes.push_back((blockinfo->id >> in) & 0xff);
        in += BYTE_SIZE;
    }

    maxLen += idLen + 1;

    // Note: Below is unneccesary, but very useful, so I kept it commented for now
    
    /*
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
    */
    
    return fullRes;
}

void Block::deserialize(const std::vector<unsigned char>& value)
{
    // I don't believe we need to use hton- because bit shifting is the same independently
    constexpr int BYTE_SIZE = sizeof(uint8_t) * 8;
    const uint8_t idSize = value.at(0);

    // Values
    uint32_t fullId = 0;
    
    // Deserialize the id
    for (uint8_t i = 0; i < idSize; ++i)
    {
        uint8_t val = value.at(i + 1);
        fullId |= val<<(i*BYTE_SIZE);
    }

    setBlockId(fullId);
}
