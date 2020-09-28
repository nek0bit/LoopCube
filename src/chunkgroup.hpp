#ifndef CHUNKGROUP_HPP
#define CHUNKGROUP_HPP
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "chunk.hpp"
#include "constants.hpp"
#include "texturehandler.hpp"
#include "structure.hpp"
#include "tree.hpp"

// TODO non-important Rename this properly
class Chunk_Group {
public:
    Chunk_Group() = default;
    Chunk_Group(unsigned long int seed, SDL_Renderer* renderer, Camera &camera, TextureHandler &textures);
    ~Chunk_Group();

    void generate_chunk(int id, std::vector<Structure*>& structure);
    void check_area(int x, std::vector<Structure*>& structure);
    void update_viewport();
    void render_all();
    void update_all();
    void render_all_viewport();
    void update_all_viewport();
    void print_chunks_fancy();

    int get_id(int surrounding = 8);

    Chunk* get_chunk_at(int x, bool loaded);

    std::vector<Chunk>& get_chunks();
    std::vector<Chunk*>& get_viewport_chunks();
    bool chunk_already_generated(int id);
private:
    void sort_all();
    template <class T, class U>
    void insert_sorted(T &data, U value) {
        typename T::iterator it = std::upper_bound(data.begin(), data.end(), value);
        data.insert(it, value);
    }
    
    std::vector<Chunk> group; // Chunks the game works with in place
    std::vector<int> loaded_chunks; // group ids
    std::vector<Chunk> group_past; // Unloaded chunks, gets moved into group when loaded again
    std::vector<int> past_chunks; // group_past ids
    std::vector<Chunk*> viewport_chunks; // Chunks that should be focused on
    unsigned long seed;

    SDL_Renderer* renderer;
    Camera* camera;
    TextureHandler* textures;
};


#endif // CHUNKGROUP_HPP
