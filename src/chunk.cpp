#include "chunk.hpp"

Chunk::Chunk(long int x, long int y)
    : x{x},
      y{y},
      MAX_WIDTH{constants::chunkWidth},
      MAX_HEIGHT{constants::chunkHeight},
      chunk{}
{
    generateChunk();

    chunk.resize(MAX_WIDTH*MAX_HEIGHT, nullptr);
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
void Chunk::renderInfo(SDL_Renderer* renderer, Camera& camera) const
{
    if (chunkInView())
    {
        constexpr int lineSize = 3;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_Rect leftLine{getChunkX() * constants::blockW,
            getChunkY() * constants::blockH,
            lineSize,
            CHUNK_H * constants::blockH};
        SDL_Rect topLine{getChunkX(0) * constants::blockW,
            getChunkY(0) * constants::blockH,
            CHUNK_W * constants::blockW,
            lineSize};
        SDL_Rect rightLine{leftLine.x + (CHUNK_W * constants::blockW) - lineSize,
            leftLine.y,
            lineSize,
            leftLine.h};
        SDL_Rect bottomLine{topLine.x,
            topLine.y + (CHUNK_H * constants::blockH) - lineSize,
            topLine.w,
            lineSize};
        SDL_RenderFillRect(renderer, &leftLine);
        SDL_RenderFillRect(renderer, &topLine);
        SDL_RenderFillRect(renderer, &rightLine);
        SDL_RenderFillRect(renderer, &bottomLine);
    }
}

void Chunk::renderAllShadows(SDL_Renderer* renderer, Camera& camera) const
{
    iterateFunctor(camera, [&](Block& blk) {
        blk.renderShadow(renderer, camera);
    });
}

void Chunk::renderAllBlocks(SDL_Renderer* renderer, Camera& camera) const
{
    iterateFunctor(camera, [&](Block& blk) {
        blk.render(renderer, camera);
    });
}

bool Chunk::placeBlock(unsigned int id, unsigned int x, unsigned int y)
{
    
}

void Chunk::placeBlockFast(unsigned int id, unsigned int x, unsigned int y)
{

}

const BlockInfo* Chunk::destroyBlock(unsigned int x, unsigned int y, Inventory& inv)
{

}

void Chunk::generateChunk()
{

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

bool Chunk::chunkInView() const
{
    SDL_Rect windowRect{camera.x, camera.y, camera.width, camera.height};
    return Generic::AABB(getChunkRect(), windowRect);
}

SDL_Rect Chunk::getChunkRect(Camera& camera) const
{
    return SDL_Rect{getChunkX() + camera.x,
        getChunkY() + camera.y,
        (CHUNK_WIDTH * constants::blockW),
        (CHUNK_HEIGHT * constants::blockH)};
}

void Chunk::iterateFunctor(Camera& camera, std::function<void(Block&)> call)
{
    // Check if chunk is in view
    if (chunkInView())
    {
        for (std::unique_ptr<Block>& block: chunk)
        {
            if (!block->shouldCull(camera))
            {
                call(*block);
            }
        }
    }
}
