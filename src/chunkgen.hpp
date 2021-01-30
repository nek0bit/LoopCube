#pragma once

#include <functional>

#include "constants.hpp"

struct ChunkGen
{
    ChunkGen(int seed = 0);
    virtual ~ChunkGen() = default;

    virtual void generateChunk(std::function<void(unsigned, unsigned, unsigned)> placeBlockFunctor,
                               const unsigned WIDTH,
                               const unsigned HEIGHT);
    
    int seed;
};
