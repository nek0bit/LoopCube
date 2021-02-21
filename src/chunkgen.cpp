#include "chunkgen.hpp"

ChunkGen::ChunkGen(int seed)
    : seed{seed}
{}

void ChunkGen::generateChunk(std::function<void(unsigned, unsigned, unsigned)> placeBlockFunctor,
                             const long chunkX,
                             const long chunkY,
                             const unsigned WIDTH,
                             const unsigned HEIGHT)
{
    // Don't spawn within player spawn
    if (!(chunkX == 0 && chunkY == 0))
    {
        for (unsigned int y = 0; y < HEIGHT; ++y)
        {
            for (unsigned int x = 0; x < WIDTH; ++x)
            {
                placeBlockFunctor(BLOCK_MOON_BLOCK, x, y);
            }
        }
    }
}
