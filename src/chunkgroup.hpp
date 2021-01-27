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

struct LoadPtr
{
    int x; // Shifts left to right
    int y; // Shifts top to bottom
};

struct ChunkData
{
    bool generated;
    std::shared_ptr<Chunk> data = nullptr;
};

//**************************************************

struct _ChunkDataSplit
{
    _ChunkDataSplit(long int x, LoadPtr& loadPtr, LoadDistance& loadDistance);
    ~_ChunkDataSplit() = default;

    void updateLoaded();
    std::unordered_map<long int, ChunkData>::iterator checkGenerate(long int y);
    void updateSplit(Camera& camera);
    void renderSplit(SDL_Renderer* renderer, TextureHandler& textures, Camera& camera);
    
    std::shared_ptr<Chunk> getData(long int y);
    
    LoadPtr& loadPtr;
    LoadDistance& loadDistance;
    std::vector<std::shared_ptr<Chunk>> loadedChunks;
private:
    void prepareLoaded();
    const long int x;
    // int = y
    std::unordered_map<long int, ChunkData> data;
};

//**************************************************

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

//**************************************************

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
