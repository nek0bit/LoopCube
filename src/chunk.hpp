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
    std::shared_ptr<Chunk> left;
    std::shared_ptr<Chunk> right;
    std::shared_ptr<Chunk> top;
    std::shared_ptr<Chunk> bottom;
};

// 2D array[y][x] flattened to 1D array
typedef std::vector<std::shared_ptr<Block>> t_blockCollection;

struct Chunk
{
    Chunk(std::shared_ptr<ChunkGen> chunkGen, long int x, long int y);
    ~Chunk();

#ifndef __HEADLESS
    void updateAll(Camera& camera);

    // Renderer
    void renderInfo(SDL_Renderer* renderer, Camera& camera);
    void renderAllShadows(SDL_Renderer* renderer, Camera& camera);
    void renderAllBlocks(SDL_Renderer* renderer, TextureHandler& textures, Camera& camera);
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
    std::shared_ptr<Block> getBorderBlock(const int x, const int y) const;
    
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
#ifndef __HEADLESS
    bool chunkInView(Camera& camera) const;
    SDL_Rect getChunkRect(Camera& camera) const;
    void iterateFunctor(Camera& camera, std::function<void(Block&)> call);
#endif

    std::shared_ptr<ChunkGen> chunkGen;
};
