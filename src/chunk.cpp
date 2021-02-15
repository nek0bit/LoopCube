#include "chunk.hpp"

Chunk::Chunk(std::shared_ptr<ChunkGen> chunkGen, long int x, long int y)
    : x{x},
      y{y},
      MAX_WIDTH{static_cast<unsigned>(constants::chunkWidth)},
      MAX_HEIGHT{static_cast<unsigned>(constants::chunkHeight)},
      chunkGen{chunkGen},
      data{},
      borders{nullptr, nullptr, nullptr, nullptr}
{
    data.resize(MAX_WIDTH*MAX_HEIGHT, nullptr);

    generateChunk();
}

Chunk::~Chunk()
{}

#ifndef __HEADLESS
void Chunk::updateAll(Camera& camera)
{
    iterateFunctor(camera, [&](Block& blk) {
        blk.update();
    });
}

// TODO render text
void Chunk::renderInfo(SDL_Renderer* renderer, Camera& camera)
{
    if (chunkInView(camera))
    {
        constexpr int lineSize = 3;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        

        SDL_Rect leftLine{static_cast<int>(getChunkX() * constants::blockW + camera.x),
            static_cast<int>(getChunkY() * constants::blockH + camera.y),
            lineSize,
            static_cast<int>(MAX_HEIGHT * constants::blockH)};
        SDL_Rect topLine{static_cast<int>(getChunkX(0) * constants::blockW + camera.x),
            static_cast<int>(getChunkY(0) * constants::blockH + camera.y),
            static_cast<int>(MAX_WIDTH * constants::blockW),
            lineSize};
        SDL_Rect rightLine{static_cast<int>(leftLine.x + (MAX_WIDTH * constants::blockW) - lineSize),
            leftLine.y,
            lineSize,
            leftLine.h};
        SDL_Rect bottomLine{topLine.x,
            static_cast<int>(topLine.y + (MAX_HEIGHT * constants::blockH) - lineSize),
            topLine.w,
            lineSize};
        SDL_RenderFillRect(renderer, &leftLine);
        SDL_RenderFillRect(renderer, &topLine);
        SDL_RenderFillRect(renderer, &rightLine);
        SDL_RenderFillRect(renderer, &bottomLine);
    }
}

// Note: I don't recommend this function as it's unoptimized and doesn't cull properly
// so it basically multiplies the amount of render calls
// If only SDL had a feature to mesh multiple blocks together :(
// I think its possible with textures but it would take up a bit of memory
void Chunk::renderAllShadows(SDL_Renderer* renderer, Camera& camera)
{
    iterateFunctor(camera, [&](Block& blk) {
        blk.renderShadow(renderer, camera);
    });
}

void Chunk::renderAllBlocks(SDL_Renderer* renderer, TextureHandler& textures, Camera& camera)
{
    iterateFunctor(camera, [&](Block& blk) {
        blk.render(renderer, textures, camera);
    });
}
#endif

// Does checking, Slower (but not by that much)
bool Chunk::placeBlock(unsigned int id, unsigned int x, unsigned int y)
{
    if (!(x > MAX_WIDTH && y > MAX_HEIGHT))
    {
        placeBlockFast(id, x, y);
        return true;
    }
    return false;
}

// Does not check, Faster. (but not by much; useful for chunk generation)
// Is actually used by the slower placeBlock
void Chunk::placeBlockFast(unsigned int id, unsigned int x, unsigned int y)
{    
    data[posToIndex(x, y)] = std::make_shared<Block>(
        Block{static_cast<int>(id), static_cast<int>(getChunkX(x)), static_cast<int>(getChunkY(y)), 4});

    updateBlockBorders(x, y, true);
}

const BlockInfo* Chunk::destroyBlock(unsigned int x, unsigned int y)
{
    std::shared_ptr<Block>& block = data[posToIndex(x, y)];
    
    if (block == nullptr) return nullptr;
    
    const BlockInfo* info = block->blockinfo;

    // Delete the block (since its a shared ptr, it will be deleted when reassigned)
    block = nullptr;

    //regenBlockBorders();
    updateBlockBorders(x, y, true);
    
    return info;
}

void Chunk::updateBlockBorders(const int x, const int y, const bool recurseOnce)
{
    std::shared_ptr<Block> left = getBorderBlock(x - 1, y),
        right = getBorderBlock(x + 1, y),
        top = getBorderBlock(x, y - 1),
        bottom = getBorderBlock(x, y + 1),
        center = getBorderBlock(x, y);
        
    // Enjoy this ugly bit of code :)
    
    // None on all sides
    if (!left && !right && !bottom && !top && center) center->typeX = 15;

    // On all sides
    if ( left &&  right &&  bottom &&  top && center) center->typeX = 4;

    // One on each side
    if ( left && !right && !bottom && !top && center) center->typeX = 13;
    if (!left &&  right && !bottom && !top && center) center->typeX = 11;
    if (!left && !right &&  bottom && !top && center) center->typeX = 12;
    if (!left && !right && !bottom &&  top && center) center->typeX = 14;

    // On opposite sides
    if ( left &&  right && !bottom && !top && center) center->typeX = 10;
    if (!left && !right &&  bottom &&  top && center) center->typeX = 9;

    // On corners/adjacent sides
    if ( left && !right && !bottom &&  top && center) center->typeX = 8;
    if ( left && !right &&  bottom && !top && center) center->typeX = 2;
    if (!left &&  right && !bottom &&  top && center) center->typeX = 6;
    if (!left &&  right &&  bottom && !top && center) center->typeX = 0;

    // All but one side
    if (!left &&  right &&  bottom &&  top && center) center->typeX = 3;
    if ( left && !right &&  bottom &&  top && center) center->typeX = 5;
    if ( left &&  right && !bottom &&  top && center) center->typeX = 7;
    if ( left &&  right &&  bottom && !top && center) center->typeX = 1;

    // Sorry for the repetition, im lazy
#ifndef __HEADLESS
    if (center) center->updateSrc();
    if (left) left->updateSrc();
    if (right) right->updateSrc();
    if (top) top->updateSrc();
    if (bottom) bottom->updateSrc();
#endif

    if (recurseOnce)
    {
        // Go around each block and update it once more
        if (x - 1 != -1) updateBlockBorders(x - 1, y, false);
        else if (borders.left) borders.left->updateBlockBorders(x - 1 + constants::chunkWidth, y, false);

        if (x + 1 != constants::chunkWidth) updateBlockBorders(x + 1, y, false);
        else if (borders.right) borders.right->updateBlockBorders(0, y, false);

        if (y - 1 != -1) updateBlockBorders(x, y - 1, false);
        else if (borders.top) borders.top->updateBlockBorders(x, y - 1 + constants::chunkHeight, false);

        if (y + 1 != constants::chunkHeight) updateBlockBorders(x, y + 1, false);
        else if (borders.bottom) borders.bottom->updateBlockBorders(x, 0, false);
    }
}

std::shared_ptr<Block> Chunk::getBorderBlock(const int x, const int y) const
{
    try
    {
        // Return a block within this chunk like normal
        if (x >= 0 && x < constants::chunkWidth &&
            y >= 0 && y < constants::chunkHeight)
        {
            return data.at(posToIndex(x, y));
        }

        // Return fixed chunk at left chunk
        if (x < 0)
        {
            if (borders.left == nullptr) return nullptr;
            return borders.left->data.at(posToIndex(x + constants::chunkWidth, y));
        }

        // Return fixed chunk at right chunk
        if (x >= constants::chunkWidth)
        {
            if (borders.right == nullptr) return nullptr;
            return borders.right->data.at(posToIndex(x - constants::chunkWidth, y));
        }

        // Return fixed chunk at top chunk
        if (y < 0)
        {
            if (borders.top == nullptr) return nullptr;
            return borders.top->data.at(posToIndex(x, y + constants::chunkHeight));
        }

        // Return fixed chunk at bottom chunk
        if (y >= constants::chunkHeight)
        {
            if (borders.bottom == nullptr) return nullptr;
            return borders.bottom->data.at(posToIndex(x, y - constants::chunkHeight));
        }
    }
    catch(const std::out_of_range& err)
    {
        std::cout << "Warning: Out of range in Chunk::getBorderBlock, this shouldn't happen" << std::endl;
        return nullptr;
    }

    return nullptr;
}

void Chunk::generateChunk()
{
    chunkGen->generateChunk([&](unsigned id, unsigned x, unsigned y)->void {
        placeBlock(id, x, y);
    }, MAX_WIDTH, MAX_HEIGHT);

}

// A little slow, not recommended except for when joining chunks
void Chunk::regenBlockBorders()
{
    for (unsigned int i = 0; i < constants::chunkWidth; ++i)
    {
        for (unsigned int j = 0; j < constants::chunkHeight; ++j)
        {
            if (data.at(posToIndex(i, j)) != nullptr) updateBlockBorders(i, j, false);
        }
    }
}

long int Chunk::getChunkX(const int x) const
{
    return (this->x * MAX_WIDTH) + x;
}

long int Chunk::getChunkY(const int y) const
{
    return (this->y * MAX_HEIGHT) + y;
}

size_t Chunk::posToIndex(const unsigned int x, const unsigned int y) const
{
    return x + y * MAX_WIDTH;
}

#ifndef __HEADLESS
bool Chunk::chunkInView(Camera& camera) const
{
    SDL_Rect windowRect{0, 0, camera.getWidth(), camera.getHeight()};
    return Generic::collision(getChunkRect(camera), windowRect);
}

SDL_Rect Chunk::getChunkRect(Camera& camera) const
{
    return SDL_Rect{static_cast<int>(getChunkX() * constants::blockW + camera.x),
        static_cast<int>(getChunkY() * constants::blockH + camera.y),
        static_cast<int>(MAX_WIDTH * constants::blockW),
        static_cast<int>(MAX_HEIGHT * constants::blockH)};
}

void Chunk::iterateFunctor(Camera& camera, std::function<void(Block&)> call)
{
    // Check if chunk is in view
    if (chunkInView(camera))
    {
        for (std::shared_ptr<Block>& block: data)
        {
            if (block == nullptr) continue;
            if (!block->shouldCull(camera))
            {
                call(*block);
            }
        }
    }
}
#endif
