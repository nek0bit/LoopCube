#ifndef CHUNK_HPP
#define CHUNK_HPP
#include <iostream>
#include <string>
#include <array>
#include <cstdlib>
#include <random>
#include <time.h>
#include <memory>
#include <functional>

#include <SDL2/SDL.h>

#include "../libs/FastNoiseLite.h"

#include "texturehandler.hpp"
#include "constants.hpp"
#include "block.hpp"
#include "camera.hpp"
#include "inventory.hpp"
#include "texturehandler.hpp"
#include "structure.hpp"
#include "tree.hpp"
#include "text.hpp"
#include "position.hpp"

class Chunk {

public:
	Chunk(unsigned long int seed, int slot, std::vector<Structure*>& structure);
	~Chunk();
	// For std::sort
	bool operator<(const Chunk &c) const;
	bool operator>(const Chunk &c) const;

	void update_all(Camera& camera);
	void render_info(SDL_Renderer* renderer, Camera& camera);
	void render_all_shadows(SDL_Renderer* renderer, Camera& camera);
	void render_all_blocks(SDL_Renderer* renderer, TextureHandler* textures, Camera& camera);
	bool place_block(int id, int x, int y);
	void place_block_raw(int id, int x, int y);
	const BlockInfo* destroy_block(int x, int y, Inventory *inv);
	void generate_chunk(unsigned long int seed, std::vector<Structure*>& structure);

	int get_slot() const;
	Position get_pos() const;

	// Need to be able to view the chunk to do stuff such as collision detection
	std::vector<std::vector<Block>>& get_chunk();
	int get_chunk_x(int x);
	int get_chunk_y(int y);
	int get_y_split(double y);
private:
	void render_all_functor(Camera& camera, std::function<void(Block&)> call);
	void debug_chunk_split();
	int get_chunk_max_size();
	
	int MAX_WIDTH;
	int MAX_HEIGHT;
	int MAX_SPLIT_COUNT;
	int MAX_SPLIT_HEIGHT;
	std::vector<std::vector<Block>> chunk;
	int slot;
	//PerlinNoise terrain_gen;
	FastNoiseLite terrain_gen;

	//std::shared_ptr<Text> chunk_text;
};

#endif // CHUNK_HPP
