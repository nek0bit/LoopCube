#include "chunk.hpp"

Chunk::Chunk(unsigned long int seed, int slot, TextureHandler &textures, std::vector<Structure*>& structures)
    : MAX_WIDTH{8}, MAX_HEIGHT{128}, terrain_gen{seed} {
    this->textures = &textures;
    this->slot = slot;
    generate_chunk(seed, structures);
}

Chunk::~Chunk() {
}

bool Chunk::operator<(const Chunk &c) const {
    return slot < c.slot;
}

bool Chunk::operator>(const Chunk &c) const {
    return slot > c.slot;
}

int Chunk::get_chunk_x(int x) {
    return x+(slot*MAX_WIDTH);
}

int Chunk::get_chunk_max_size() {
    return MAX_WIDTH*MAX_HEIGHT;
}

int Chunk::get_slot() const {
    return slot;
}

std::vector<Block>& Chunk::get_chunk() {
    return chunk;
}

void Chunk::generate_chunk(unsigned long int seed, std::vector<Structure*>& structures) {
    // Start rng for structures
    std::random_device dev;
    std::mt19937 rng(seed);
    std::uniform_int_distribution<std::mt19937::result_type> dist(-5,5);
    
    for (auto i = 0; i < std::abs(slot); ++i) dist(rng);

    for (int x = 1; x < MAX_WIDTH+1; ++x) {
        double d_x = (double)x/(double)MAX_WIDTH;

        // Generate world
        int temp = floor(terrain_gen.noise((d_x+slot) * .3, 0)*12);

        int offset = 30;
        for (int y = 0; y < MAX_HEIGHT-temp-offset; ++y) {
            double d_y = (double)y/(double)MAX_HEIGHT;
            if (y == 0) {
                place_block(BLOCK_GRASS, x, y+temp+offset);
                if (dist(rng) == 0) structures.push_back(new Tree(get_chunk_x(x)-8, y+temp+offset));
            } else if (y >= 1 && y <= 3) {
                place_block(BLOCK_DIRT, x, y+temp+offset);
            } else {
                int cave_noise = terrain_gen.noise(d_x+(slot), d_y*12)*400;
                if (cave_noise < 150 && cave_noise > -150) {
                    place_block(BLOCK_STONE, x, y+temp+offset);
                }
            }
        }
    }

}

const BlockInfo* Chunk::destroy_block(int x, int y, Inventory *inv) {
    for (auto i = chunk.begin(); i < chunk.end(); ++i) {
        if (get_chunk_x(x) == i->get_default_x() && y == i->get_default_y()) {

            // Get blockinfo
            const BlockInfo* info = i->get_blockinfo();
            inv->add_item(info->get_id());
            
            chunk.erase(i);
            return info;
        }
    }
    return nullptr;
}

void Chunk::place_block(int id, int x, int y) {
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
            // Place the block
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


void Chunk::render_all_shadows(SDL_Renderer* renderer, Camera& camera) {
    // Then render blocks
    for(size_t j = 0; j < chunk.size(); ++j) {
        if (!chunk[j].out_of_view(camera)) {
            chunk[j].render_shadow(renderer);
        }
    }
}

void Chunk::render_all_blocks(SDL_Renderer* renderer, Camera& camera) {
    // Then render blocks
    for(size_t j = 0; j < chunk.size(); ++j) {
        if (!chunk[j].out_of_view(camera)) {
            chunk[j].render(renderer);
        }
    }
}
