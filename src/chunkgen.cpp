#include "chunkgen.hpp"

ChunkGen::ChunkGen(int seed)
    : seed{seed}
{}

void ChunkGen::generateChunk(std::function<void(unsigned, unsigned, unsigned)> placeBlockFunctor,
                             const unsigned WIDTH,
                             const unsigned HEIGHT)
{
    for (unsigned int y = 0; y < WIDTH; ++y)
    {
        for (unsigned int x = 0; x < HEIGHT; ++x)
        {
            if ((x+1) * (y+1) == WIDTH*HEIGHT)
            {
                placeBlockFunctor(BLOCK_WOOD, x, y); // Temporarily make the last block unique!!
            }
            else
            {
                placeBlockFunctor(BLOCK_MOON_BLOCK, x, y);
            }
        }
    }
}
