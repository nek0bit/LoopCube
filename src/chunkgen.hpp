#pragma once

#include "chunk.hpp"

struct ChunkGen
{
    ChunkGen(int seed = 0);
    virtual ~ChunkGen() = default;

    virtual void generateChunk(Chunk&);
    
    int seed;
}
