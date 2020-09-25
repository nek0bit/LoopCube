#include "chunkgroup.hpp"

// I could probably move the renderer and camera out of the class, but it doesn't take much memory so I think it's fine
Chunk_Group::Chunk_Group(unsigned long seed, SDL_Renderer* renderer, Camera &camera, TextureHandler &textures) {
    this->renderer = renderer;
    this->camera = &camera;
    this->textures = &textures;
    this->seed = seed;
}

Chunk_Group::~Chunk_Group() {

}

std::vector<Chunk>& Chunk_Group::get_chunks() {
    return group;
}

bool Chunk_Group::chunk_already_generated(int id) {
    bool check = false;
    for (auto &i: past_chunks) {
        if (i == id) {
            check = true;
            break;
        }
    }
    return check;
}

void Chunk_Group::generate_chunk(int id) {
    bool check = chunk_already_generated(id);
    // Generate the chunk if it hasn't been generated before
    if (std::find(loaded_chunks.begin(), loaded_chunks.end(), id) == loaded_chunks.end() && !check) {
        Chunk temp_chunk(seed, id, *textures);
        group.push_back(temp_chunk);

        loaded_chunks.push_back(id);
    }

    // If chunk has been generated before, just reload an older chunk
    if (check) {
        // Get the index of the last unloaded chunk
        auto it_past_chunks = std::find(past_chunks.begin(), past_chunks.end(), id);
        int chunk_index = std::distance(past_chunks.begin(), it_past_chunks);
        try {
            // Add chunk back
            group.push_back(group_past.at(chunk_index));
            loaded_chunks.push_back(id);
            // Remove chunk from past_chunks to prevent from infinitely loaded chunks
            group_past.erase(group_past.begin() + chunk_index);
            past_chunks.erase(past_chunks.begin() + chunk_index);

        } catch (std::out_of_range &e) {
            std::cout << "[LOG] std::out_of_range handled: " << e.what() << std::endl;
        }
    }
}

void Chunk_Group::check_area(int x) {
#if defined(__WIIU__) || defined(__SWITCH__)
    const int load_distance = 4;
#else
    const int load_distance = 6;
#endif
    
    int id = 0;
    id = ceil((x) / (8 * constants::block_w));

    // Unload old chunks
    if (loaded_chunks.size() > load_distance) {
        // Unloaded chunks off of rightside of screen
        /* We basically just move the chunk from group to group_past
           Since we want to reload these chunks when the screen gets back in view */
        if (id-load_distance-1 < loaded_chunks[0]) {
            group_past.push_back(*(group.end()-1));
            past_chunks.push_back(*(loaded_chunks.end()-1));


            loaded_chunks.erase(loaded_chunks.end()-1);
            group.erase(group.end()-1);
        }

        // Unload chunks from leftside of screen
        if (id+load_distance > loaded_chunks[loaded_chunks.size()-1]) {
            group_past.push_back(*(group.begin()));
            past_chunks.push_back(*(loaded_chunks.begin()));
            loaded_chunks.erase(loaded_chunks.begin());
            group.erase(group.begin());
        }
    }

    for (int i = load_distance*-1; i < load_distance; ++i) {
        generate_chunk(id+i);
    }
}

void Chunk_Group::sort_all() {
    std::sort(loaded_chunks.begin(), loaded_chunks.end());
    std::sort(group.begin(), group.end());
    std::sort(group_past.begin(), group_past.end());
    std::sort(past_chunks.begin(), past_chunks.end());
}

Chunk* Chunk_Group::get_chunk_at(int x) {
    int id = 0;
    id = ceil((x*constants::block_w) / (8 * constants::block_w));

    std::vector<int>::iterator hovered_chunk = std::find(loaded_chunks.begin(), loaded_chunks.end(), id);

    if (hovered_chunk != loaded_chunks.end()) {
        int chunk_index = std::distance(loaded_chunks.begin(), hovered_chunk);
        return &group[chunk_index];
    }
    return nullptr;
}

int Chunk_Group::get_id(int surrounding) {
    double id = 0;
    id = ceil((camera->get_x() - (camera->get_width()/2))  / (surrounding * constants::block_w));
    if (id > 0) {
        id *= -1;
    } else {
        id = abs(id);
    }
    return id;
}

void Chunk_Group::render_all() {
    for (auto &chunk: group) {
        chunk.render_all_shadows(renderer, *camera);
    }
    for (auto &chunk: group) {
        chunk.render_all_blocks(renderer, *camera);
    }
}

void Chunk_Group::update_all() {
    for (auto &chunk: group) {
        chunk.update_all(*camera);
    }
}
