#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <unordered_map>

#ifndef __HEADLESS
#include <SDL2/SDL.h>
#include "size.hpp"
#include <glm/glm.hpp>
#include "texturehandler.hpp"
#endif

#include "api.hpp"
#include "chunkgen.hpp"
#include "chunk.hpp"
#include "constants.hpp"
#include "generic.hpp"

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
    std::shared_ptr<Chunk> data;
};

//**************************************************

struct _ChunkDataSplit
{
    _ChunkDataSplit(long int x, LoadPtr& loadPtr, LoadDistance& loadDistance,
                    int& fd, bool& isFdSet, bool& chunkReady, ChunkPos& pendingChunk, GLuint shader);
    _ChunkDataSplit(long int x, LoadPtr& loadPtr, LoadDistance& loadDistance,
                    std::shared_ptr<ChunkGen> chunkGen, int& fd,
                    bool& isFdSet, bool& chunkReady, ChunkPos& pendingChunk, GLuint shader);
    ~_ChunkDataSplit();

    void updateLoaded();
    std::unordered_map<long int, ChunkData>::iterator checkGenerate(long int y,
                                                                    std::shared_ptr<Chunk> toInsert,
                                                                    bool isClient = false);
#ifndef __HEADLESS
    void updateSplit(const Camera& camera);
    void renderSplit(const Graphics& renderer, const Camera& camera) const;
#endif

    void updateBorderedChunks(std::unordered_map<long int, ChunkData>::iterator current, const long y);
    
    Chunk* getData(long int y);
    
    LoadPtr& loadPtr;
    LoadDistance& loadDistance;
    std::vector<Chunk*> loadedChunks;

    // Access pointers
    _ChunkDataSplit* left;
    _ChunkDataSplit* right;
    
    const long int x;
    std::unordered_map<long int, ChunkData> data;
    bool& chunkReady;
    ChunkPos& pendingChunk;
private:
    GLuint shader;
    bool& isFdSet;
    int& fd;
    bool isClient;
    void prepareLoaded();
    std::shared_ptr<ChunkGen> chunkGen;
    // int = y
};

//**************************************************

struct ChunkGroup
{
    ChunkGroup(GLuint shader = 0);
    ChunkGroup(std::shared_ptr<ChunkGen> chunkGen, GLuint shader = 0);
    ~ChunkGroup();

#ifndef __HEADLESS
    void update(const Camera& camera);
    void render(const Graphics& renderer, const Camera& camera) const;
#endif

    Chunk* getChunkAt(const long x, const long y);
    std::vector<Chunk*> isWithinChunks(const glm::vec2& vec, const glm::vec2& size);
    void generateChunkAt(const long x, const long y);
    void loadFromDeserialize(std::vector<unsigned char>& value, int start = 1);

    void setFd(const int fd);

    ChunkPos posToChunkPos(const double x, const double y) const;

    LoadPtr loadPtr;
    LoadDistance& loadDistance;
    bool chunkReady;
private:
    GLuint shader;
    ChunkPos pendingChunk;
    bool isFdSet;
    int fd;
    bool isClient; // Client chunks work differently from server chunks
    std::unordered_map<long int, std::shared_ptr<_ChunkDataSplit>>::iterator
        checkSplitGenerate(long int x);
    void updateLoaded();
    void prepareLoaded();
    _ChunkDataSplit* getData(long int x);
    
    std::shared_ptr<ChunkGen> chunkGen;
    std::vector<_ChunkDataSplit*> loadedSplits;
    std::unordered_map<long int, std::shared_ptr<_ChunkDataSplit>> data;
};
