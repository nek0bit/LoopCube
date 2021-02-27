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

void Block::renderShadow(const Graphics& graphics, const Camera& camera) const
{
    const Vec2 val = getPos(camera);
	SDL_Rect shadow{static_cast<int>(val.x + 5), static_cast<int>(val.y + 5),
		static_cast<int>(size.w), static_cast<int>(size.h)};
    
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 60);
    //SDL_RenderFillRect(renderer, &shadow);
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
    std::vector<uint8_t> fullRes;
    const uint8_t idLen = sizeof(blockinfo->id); // For now, we'll kind of use fixed lengths

    // Push back the id so we can know its length for deserialization!
    fullRes.push_back(idLen);

    Generic::serializeUnsigned(blockinfo->id, idLen, [&fullRes](uint8_t back)->void {
            fullRes.push_back(back);
        });
    
    return fullRes;
}

void Block::deserialize(const std::vector<unsigned char>& value)
{
    // I don't believe we need to use hton- because bit shifting is the same independently
    const uint8_t idSize = value.at(0);

    try
    {
        setBlockId(Generic::deserializeUnsigned<std::vector<uint8_t>, uint8_t>(value, 1, idSize));
    }
    catch (const std::out_of_range& err)
    {
        std::cerr << "Failed to deserialize block: " << err.what() << std::endl;
    }
}
