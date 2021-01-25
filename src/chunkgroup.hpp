#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "chunk.hpp"
#include "vector.hpp"
#include "size.hpp"
#include "constants.hpp"
#include "generic.hpp"
#include "texturehandler.hpp"

struct _ChunkDataSplit
{
    
};

struct _ChunkData
{
    
};

struct ChunkGroup
{
    ChunkGroup();
    ~ChunkGroup();
    

private:
    void generateChunk(int x, int y);
    
    _ChunkData data;
};
