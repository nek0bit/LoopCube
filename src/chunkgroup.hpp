#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "chunkgen.hpp"
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

struct ChunkPos
{
    long int x;
    long int y;
};

struct ChunkData
{
    bool generated;
    std::shared_ptr<Chunk> data = nullptr;
};

//**************************************************

struct _ChunkDataSplit
{
    _ChunkDataSplit(long int x, LoadPtr& loadPtr, LoadDistance& loadDistance,
                    std::shared_ptr<ChunkGen> chunkGen);
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
    std::shared_ptr<ChunkGen> chunkGen;
    // int = y
    std::unordered_map<long int, ChunkData> data;
};

//**************************************************

struct ChunkGroup
{
    ChunkGroup(std::shared_ptr<ChunkGen> chunkGen);
    ~ChunkGroup() = default;

    void update(Camera& camera);
    void render(SDL_Renderer* renderer, TextureHandler& textures, Camera& camera);

    std::shared_ptr<Chunk> getChunkAt(const long int x, const long int y);

    ChunkPos posToChunkPos(double x, double y) const;

    LoadPtr loadPtr;
    LoadDistance& loadDistance;
private:
    std::unordered_map<long int, std::shared_ptr<_ChunkDataSplit>>::iterator
        checkSplitGenerate(long int x);
    void updateLoaded();
    void prepareLoaded();
    std::shared_ptr<_ChunkDataSplit> getData(long int x);
    
    std::shared_ptr<ChunkGen> chunkGen;
    std::vector<std::shared_ptr<_ChunkDataSplit>> loadedSplits;
    std::unordered_map<long int, std::shared_ptr<_ChunkDataSplit>> data;
};
