#ifndef CHUNK_HPP
#define CHUNK_HPP
#include <iostream>
#include <string>
#include <array>
#include <cstdlib>
#include <time.h>
#include <SDL2/SDL.h>

#include "constants.hpp"
#include "block.hpp"
#include "perlin.hpp"
#include "camera.hpp"
#include "inventory.hpp"
#include "texturehandler.hpp"

class Chunk {

public:
    Chunk(unsigned long int seed, int slot, TextureHandler &texture);
    ~Chunk();
    // For std::sort
    bool operator<(const Chunk &c);

    void update_all(Camera& camera);
    void render_all(Camera& camera, SDL_Renderer* renderer);
    void place_block(std::string id, int x, int y);
    void destroy_block(int x, int y, Inventory *inv);
    void generate_chunk();

    int get_slot();

    // Need to be able to view the chunk to do stuff such as collision detection
    std::vector<Block>* get_chunk();

private:
    int get_chunk_x(int x);
    int get_chunk_max_size();

    int MAX_WIDTH;
    int MAX_HEIGHT;
    std::vector<Block> chunk;
    int slot;
    PerlinNoise terrain_gen;

    TextureHandler* textures;
};


#endif // CHUNK_HPP
