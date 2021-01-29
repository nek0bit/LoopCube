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
            placeBlockFunctor(1, x, y);
        }
    }
}
