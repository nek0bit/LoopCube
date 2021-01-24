#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>

#include <SDL2/SDL.h>

#include "generic.hpp"
#include "texturehandler.hpp"
#include "constants.hpp"
#include "block.hpp"
#include "camera.hpp"
#include "inventory.hpp"
#include "vector.hpp"
#include "size.hpp"
#include "text.hpp"

// 2D array[y][x] flattened to 1D array
typedef std::vector<std::unique_ptr<Block>> t_blockCollection;

struct Chunk
{
    Chunk(long int x, long int y);
    ~Chunk();

    void updateAll(Camera& camera);

    // Renderer
    void renderInfo(SDL_Renderer* renderer, Camera& camera) const;
    void renderAllShadows(SDL_Renderer* renderer, Camera& camera) const;
    void renderAllBlocks(SDL_Renderer* renderer, TextureHandler& textures, Camera& camera) const;

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
private:
    size_t posToIndex(const unsigned int x, const unsigned int y) const;
    bool chunkInView() const;
    SDL_Rect getChunkRect(Camera& camera) const;
    void iterateFunctor(Camera& camera, std::function<void(Block&)> call);
    
    // Size
    const int MAX_WIDTH;
    const int MAX_HEIGHT;

    t_blockCollection chunk;
};
