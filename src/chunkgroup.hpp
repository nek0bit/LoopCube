#ifndef CHUNKGROUP_HPP
#define CHUNKGROUP_HPP
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <future>
#include <thread>
#include <chrono>

#include <SDL2/SDL.h>

#include "position.hpp"
#include "chunk.hpp"
#include "constants.hpp"
#include "texturehandler.hpp"
#include "structure.hpp"
#include "tree.hpp"

using namespace std::chrono_literals;

// Need to rewrite this ugly shit when verticle chunks are properly implemented, it's very complicated
class ChunkGroup {
public:
	ChunkGroup() = default;
	ChunkGroup(unsigned long int seed);
	~ChunkGroup();

	// Update
	void generate_chunk(int id, std::vector<Structure*>& structure);
	void check_area(long long int x, std::vector<Structure*>& structure);

	void update_viewport();
	void update_all_viewport(Camera& camera);

	// Render
	void render_all_viewport(SDL_Renderer* renderer, TextureHandler& textures, Camera& camera);

	// Getters
	int get_id(Camera& camera, int surrounding = 8);
	Chunk* get_chunk_at(double x, bool loaded);
	int get_chunk_y(double y);
	std::vector<Chunk>& get_chunks();
	std::vector<Chunk*>& get_viewport_chunks();
	
	bool chunk_already_generated(int id);

	// Debug
	void print_chunks_fancy();
private:
	void sort_all();
	template <class T, class U>
	void insert_sorted(T &data, U value) {
		typename T::iterator it = std::upper_bound(data.begin(), data.end(), value);
		data.insert(it, value);
	}

	// Config
	bool render_chunk_info;

	std::vector<Chunk> group; // Chunks the game works with in place
	std::vector<int> loaded_chunks; // group ids
	std::vector<Chunk> group_past; // Unloaded chunks, gets moved into group when loaded again
	std::vector<int> past_chunks; // group_past ids
	std::vector<Chunk*> viewport_chunks; // Chunks that should be focused on
	unsigned long seed;

	std::vector<std::future<Chunk>> chunk_async_loading;
};


#endif // CHUNKGROUP_HPP
