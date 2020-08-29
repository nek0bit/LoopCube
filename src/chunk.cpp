#include "chunk.hpp"

Chunk::Chunk(unsigned long int seed, int slot, TextureHandler &textures)
    : MAX_WIDTH{8}, MAX_HEIGHT{128}, terrain_gen{seed} {
    this->textures = &textures;
    this->slot = slot;
    generate_chunk();
}

Chunk::~Chunk() {
}

bool Chunk::operator<(const Chunk &c) {
    if (slot < c.slot) {
        return true;
    }
    return false;
}

int Chunk::get_chunk_x(int x) {
    return x+(slot*MAX_WIDTH);
}

int Chunk::get_chunk_max_size() {
    return MAX_WIDTH*MAX_HEIGHT;
}

int Chunk::get_slot() {
    return slot;
}

std::vector<Block>* Chunk::get_chunk() {
    return &chunk;
}

void Chunk::generate_chunk() {
    for (int x = 1; x < MAX_WIDTH+1; ++x) {
        double d_x = (double)x/(double)MAX_WIDTH;

        // Generate world
        int temp = floor(terrain_gen.noise((d_x+slot) * .3, 0)*12);

        int offset = 30;
        for (int y = 0; y < MAX_HEIGHT-temp-offset; ++y) {
            double d_y = (double)y/(double)MAX_HEIGHT;
            if (y == 0) {
                place_block("grass", x, y+temp+offset);
            } else if (y >= 1 && y <= 3) {
                place_block("dirt", x, y+temp+offset);
            } else {
                int cave_noise = terrain_gen.noise(d_x+(slot), d_y*12)*400;
                if (cave_noise < 150 && cave_noise > -150) {
                    place_block("stone", x, y+temp+offset);
                }
            }
        }
    }

}

void Chunk::destroy_block(int x, int y, Inventory *inv) {
    for (auto i = chunk.begin(); i < chunk.end(); ++i) {
        if (get_chunk_x(x) == i->get_default_x() && y == i->get_default_y()) {

            // Get blockinfo
            const BlockInfo* info = i->get_blockinfo();
            inv->add_item(std::string(info->get_id()));
            
            chunk.erase(i);
            break;
        }
    }
}

void Chunk::place_block(std::string id, int x, int y) {
    Block temp_block{id, *textures, get_chunk_x(x), y};
    // Check if between chunk size
    if (x < MAX_WIDTH+1 && x >= 0 && y < MAX_HEIGHT+1 && y >= 0) {
        // Check if a block has been placed here before
        bool is_duplicate = false;
        for (auto &i: chunk) {
            if (get_chunk_x(x) == i.get_default_x() && y == i.get_default_y()) {
                is_duplicate = true;
                break;
            }
        }
        if (!is_duplicate) {
            chunk.push_back(temp_block);
        }
    } else {
        std::cout << "[ERROR] Block placed too far" << std::endl;
    }

}

void Chunk::update_all(Camera& camera) {
    for(size_t i = 0; i < chunk.size(); i++) {
        chunk[i].update(camera);
    }
}

void Chunk::render_all(SDL_Renderer* renderer, Camera& camera) {
    for(size_t i = 0; i < chunk.size(); i++) {
        if (!chunk[i].out_of_view(camera)) {
            chunk[i].render(renderer);
        }
    }
}
