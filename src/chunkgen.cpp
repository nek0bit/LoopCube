#include "chunkgen.hpp"

ChunkGen::ChunkGen(int seed)
    : seed{seed}
{}

void ChunkGen::generateChunk(std::function<void(unsigned, unsigned, unsigned)> placeBlockFunctor,
                             const unsigned WIDTH,
                             const unsigned HEIGHT)
{
    for (unsigned int y = 0; y < HEIGHT; ++y)
    {
        for (unsigned int x = 0; x < WIDTH; ++x)
        {
            placeBlockFunctor(BLOCK_MOON_BLOCK, x, y);
        }
    }
}
