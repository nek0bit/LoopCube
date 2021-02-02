#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>

#include <SDL2/SDL.h>

#include "chunkgen.hpp"
#include "generic.hpp"
#include "texturehandler.hpp"
#include "constants.hpp"
#include "block.hpp"
#include "camera.hpp"
#include "inventory.hpp"
#include "vector.hpp"
#include "size.hpp"
#include "text.hpp"

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

struct Chunk
{
    Chunk(std::shared_ptr<ChunkGen> chunkGen, long int x, long int y);
    ~Chunk();

    void updateAll(Camera& camera);

    // Renderer
    void renderInfo(SDL_Renderer* renderer, Camera& camera);
    void renderAllShadows(SDL_Renderer* renderer, Camera& camera);
    void renderAllBlocks(SDL_Renderer* renderer, TextureHandler& textures, Camera& camera);

    // Block modification
    bool placeBlock(unsigned int id, unsigned int x, unsigned int y);
    void placeBlockFast(unsigned int id, unsigned int x, unsigned int y);
    const BlockInfo* destroyBlock(unsigned int x, unsigned int y, Inventory& inv);

    void generateChunk(); // Temporary

    // Common methods
    long int getChunkX(const int x = 0) const;
    long int getChunkY(const int y = 0) const;
    
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
    bool chunkInView(Camera& camera) const;
    SDL_Rect getChunkRect(Camera& camera) const;
    void iterateFunctor(Camera& camera, std::function<void(Block&)> call);

    std::shared_ptr<ChunkGen> chunkGen;
};
