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

// Shifts left to right
// Shifts top to bottom
struct LoadPtr
{
    int x;
    int y;
};

struct _ChunkDataSplit
{
    _ChunkDataSplit(LoadPtr& loadPtr, LoadDistance& loadDistance);
    ~_ChunkDataSplit() = default;

    std::shared_ptr<Chunk>& getData(long int y);
    
    LoadPtr& loadPtr;
    LoadDistance& loadDistance;
private:
    std::vector<std::shared_ptr<Chunk>> loadedChunks;
    // int = y
    std::unordered_map<long int, std::shared_ptr<Chunk>> data;
};

struct _ChunkData
{
    _ChunkData(LoadPtr& loadPtr, LoadDistance& loadDistance);
    ~_ChunkData() = default;

    std::shared_ptr<_ChunkDataSplit>& getData(long int x);

    LoadPtr& loadPtr;
    LoadDistance& loadDistance;
private:
    std::vector<std::shared_ptr<_ChunkDataSplit>> loadedSplits;
    // int = x
    std::unordered_map<long int, std::shared_ptr<_ChunkDataSplit>> data;
};

// TODO work on me when done with above classes
struct ChunkGroup
{
    ChunkGroup();
    ~ChunkGroup() = default;

    std::shared_ptr<Chunk>& getChunkAt(long int x, long int y);

private:
    void chunkLoad(long int x, long int y);

    _ChunkData chunks;

    // Loaded data
    LoadPtr loadPtr;
    LoadDistance loadDistance;
};
