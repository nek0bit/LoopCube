#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>

#ifndef __HEADLESS
#include <SDL2/SDL.h>
#include "texturehandler.hpp"
#include "camera.hpp"
#include "inventory.hpp"
#include "vector.hpp"
#include "size.hpp"
#include "text.hpp"
#endif

#include "block.hpp"
#include "chunkgen.hpp"
#include "generic.hpp"
#include "constants.hpp"

// forward declaration
struct Chunk;
struct BorderingChunks
{
    Chunk* left;
    Chunk* right;
    Chunk* top;
    Chunk* bottom;
};

// 2D array[y][x] flattened to 1D array
typedef std::vector<std::shared_ptr<Block>> t_blockCollection;

struct indPos
{
    long int x;
    long int y;
};

struct Chunk
{
    Chunk(std::shared_ptr<ChunkGen> chunkGen, long int x, long int y);
    ~Chunk();

#ifndef __HEADLESS
    void updateAll(const Camera& camera);

    // Renderer
    void renderInfo(const Graphics& graphics, const Camera& camera) const;
    void renderAllShadows(const Graphics& graphics, const Camera& camera) const ;
    void renderAllBlocks(const Graphics& graphics, TextureHandler& textures, const Camera& camera) const;
#endif

    // Block modification
    bool placeBlock(unsigned int id, unsigned int x, unsigned int y);
    void placeBlockFast(unsigned int id, unsigned int x, unsigned int y);
    const BlockInfo* destroyBlock(unsigned int x, unsigned int y);

    void updateBlockBorders(const int x, const int y, const bool recurseOnce = false);

    void regenBlockBorders();

    void generateChunk();

    // Common methods
    long int getChunkX(const int x = 0) const;
    long int getChunkY(const int y = 0) const;

    // Designed for updating block borders
    Block* getBorderBlock(const int x, const int y) const;

    std::vector<unsigned char> serialize() const;
    void deserialize(std::vector<unsigned char>& value, bool ignoreFirstByte = true);
    
    // Chunk position
    const long int x;
    const long int y;

    // Chunk size
    const unsigned int MAX_WIDTH;
    const unsigned int MAX_HEIGHT;

    t_blockCollection data;

    BorderingChunks borders;
private:
    size_t posToIndex(const unsigned int x, const unsigned int y) const;
    indPos indexToPos(const size_t index) const;
#ifndef __HEADLESS
    bool chunkInView(const Camera& camera) const;
    SDL_Rect getChunkRect(const Camera& camera) const;
    void iterateFunctor(const Camera& camera, const std::function<void(Block&)> call) const;
#endif

    std::shared_ptr<ChunkGen> chunkGen;
};
