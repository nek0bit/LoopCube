#pragma once

#include <functional>

struct ChunkGen
{
    ChunkGen(int seed = 0);
    virtual ~ChunkGen() = default;

    virtual void generateChunk(std::function<void(unsigned, unsigned, unsigned)> placeBlockFunctor,
                               const unsigned WIDTH,
                               const unsigned HEIGHT);
    
    int seed;
};
