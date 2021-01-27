#include "chunk.hpp"

Chunk::Chunk(long int x, long int y)
    : x{x},
      y{y},
      MAX_WIDTH{static_cast<unsigned>(constants::chunkWidth)},
      MAX_HEIGHT{static_cast<unsigned>(constants::chunkHeight)},
      chunk{}
{
    chunk.resize(MAX_WIDTH*MAX_HEIGHT, nullptr);

    generateChunk();
}

Chunk::~Chunk()
{}

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

        SDL_Rect leftLine{static_cast<int>(getChunkX() * constants::blockW),
            static_cast<int>(getChunkY() * constants::blockH),
            lineSize,
            static_cast<int>(MAX_HEIGHT * constants::blockH)};
        SDL_Rect topLine{static_cast<int>(getChunkX(0) * constants::blockW),
            static_cast<int>(getChunkY(0) * constants::blockH),
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
    chunk[posToIndex(x, y)] = std::make_shared<Block>(
        Block{static_cast<int>(id), static_cast<int>(getChunkX(x)), static_cast<int>(getChunkY(y))});
}

const BlockInfo* Chunk::destroyBlock(unsigned int x, unsigned int y, Inventory& inv)
{
    return nullptr;
}

void Chunk::generateChunk()
{
    for (unsigned int y = 0; y < MAX_HEIGHT; ++y)
    {
        for (unsigned int x = 0; x < MAX_WIDTH; ++x)
        {
            placeBlockFast(0, x, y);
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
        for (std::shared_ptr<Block>& block: chunk)
        {
            if (!block->shouldCull(camera))
            {
                call(*block);
            }
        }
    }
}
