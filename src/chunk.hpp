#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <memory>
#include <functional>

#include <SDL2/SDL.h>

#include "texturehandler.hpp"
#include "constants.hpp"
#include "block.hpp"
#include "camera.hpp"
#include "inventory.hpp"
#include "vector.hpp"
#include "size.hpp"
#include "text.hpp"

struct Chunk
{
    Chunk(long int x, long int y);
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

    // Chunk position
    const long int x;
    const long int y;
private:
    void renderAllFunctor(Camera& camera, std::function<void(Block&)> call);
    
    // Size
    const int MAX_WIDTH;
    const int MAX_HEIGHT;
};
