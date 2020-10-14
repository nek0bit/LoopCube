#include "chunk.hpp"

Chunk::Chunk(unsigned long int seed, int slot, TextureHandler &textures, std::vector<Structure*>& structures)
    : MAX_WIDTH{constants::chunk_width}, MAX_HEIGHT{constants::chunk_height}, MAX_SPLIT_COUNT{constants::chunk_split_count},
	  MAX_SPLIT_HEIGHT{constants::chunk_split_height}, terrain_gen{seed}, chunk_text{nullptr} {
    this->textures = &textures;
    this->slot = slot;

	// Setup chunks
	chunk.resize(MAX_SPLIT_COUNT, std::vector<Block>{});
	
    generate_chunk(seed, structures);
	
}

Chunk::~Chunk() {
}

void Chunk::debug_chunk_split() {
	std::cout << "======================= Chunk " << get_slot() << " =======================" << std::endl;
	for (size_t i = 0; i < chunk.size(); ++i) {
		std::cout << ">>> chunk " << i << std::endl;
		for (size_t j = 0; j < chunk[i].size(); ++j) {
			std::cout << chunk[i][j].get_default_x() << ", ";
			if ((j+1)%MAX_WIDTH == 0) std::cout << std::endl;
		}
		std::cout << std::endl;
	}
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

int Chunk::get_chunk_y(int y) {
	int y_split = y / MAX_SPLIT_HEIGHT;
	return y - (y_split*MAX_SPLIT_HEIGHT);
}

int Chunk::get_y_split(int y) {
	return y / MAX_SPLIT_HEIGHT;
}

// Warning: Obsolete
int Chunk::get_chunk_max_size() {
    return MAX_WIDTH*MAX_HEIGHT;
}

int Chunk::get_slot() const {
    return slot;
}

std::vector<std::vector<Block>>& Chunk::get_chunk() {
    return chunk;
}

void Chunk::generate_chunk(unsigned long int seed, std::vector<Structure*>& structures) {
    // Start rng for structures
    std::random_device dev;
    std::mt19937 rng(seed);
    std::uniform_int_distribution<std::mt19937::result_type> dist(-5,5);
    
    for (auto i = 0; i < std::abs(slot); ++i) dist(rng);

    for (int x = 0; x < MAX_WIDTH; ++x) {
        double d_x = (double)x/(double)MAX_WIDTH;

        // Generate world
        int temp = floor(terrain_gen.noise((d_x+slot) * .3, 0)*12);

        int offset = 30;
        for (int y = 0; y < MAX_HEIGHT-temp-offset; ++y) {
            double d_y = (double)y/(double)MAX_HEIGHT;
            if (y == 0) {
                place_block(BLOCK_GRASS, x, y+temp+offset);
                //if (dist(rng) == 0) structures.push_back(new Tree(get_chunk_x(x), y+temp+offset));
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
	int i = get_y_split(y);
	for (size_t j = 0; j < chunk[i].size(); ++j) {
		if (get_chunk_x(x) == chunk[i][j].get_obj().x && y == chunk[i][j].get_obj().y) {
			const BlockInfo* info = chunk[i][j].get_blockinfo();
			inv->add_item(info->get_id());

			chunk[i].erase(chunk[i].begin() + j);
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
		// TODO URGENT UPDATE THIS
        //for (auto &i: chunk) {
        //    if (get_chunk_x(x) == i.get_default_x() && y == i.get_default_y()) {
		//      is_duplicate = true;
		//      break;
		//  }
        //}
		int i = get_y_split(y);
		for (size_t j = 0; j < chunk[i].size(); ++j) {
			if (get_chunk_x(x) == chunk[i][j].get_default_x() && y == chunk[i][j].get_default_y()) {
				is_duplicate = true;
				break;
			}
		}
		
        if (!is_duplicate) {
            // Place the block
			int y_split = y / MAX_SPLIT_HEIGHT;
			chunk[y_split].push_back(temp_block);
        }
    } else {
        std::cout << "[ERROR] Block placed too far" << std::endl;
    }

}

// TODO Update these functions to operate on split chunks properly
void Chunk::update_all(Camera& camera) {
    for(size_t i = 0; i < chunk.size(); ++i) {
		for (size_t j = 0; j < chunk[i].size(); ++j) {
			chunk[i][j].update(camera);
		}
    }
}


void Chunk::render_all_shadows(SDL_Renderer* renderer, Camera& camera) {
    // Then render blocks
    for(size_t i = 0; i < chunk.size(); ++i) {
		for (size_t j = 0; j < chunk[i].size(); ++j) {
			if (!chunk[i][j].out_of_view(camera)) {
				chunk[i][j].render_shadow(renderer);
			}
		}
    }
}

void Chunk::render_all_blocks(SDL_Renderer* renderer, Camera& camera) {
    // Then render blocks
    for(size_t i = 0; i < chunk.size(); ++i) {
		for (size_t j = 0; j < chunk[i].size(); ++j) {
			if (!chunk[i][j].out_of_view(camera)) {
				chunk[i][j].render(renderer);
			}
		}
    }
}

void Chunk::render_info(SDL_Renderer* renderer, Camera& camera) {
	int pos = get_chunk_x(0)*constants::block_w;
	if (chunk_text == nullptr) {
		SDL_Color color;
		color.r = 100; color.g = 100; color.b = 100; color.a = 255;
		// Note: chunk_text was nullptr before of course, so this is fine
		std::string tmp = std::string{"Chunk "} + std::to_string(get_slot());
		chunk_text = std::shared_ptr<Text>(new Text(renderer, tmp, color, constants::header_font));
	} else {
		chunk_text->draw(pos+20+camera.get_x(), 70);
	}

	// Draw gap
	SDL_Rect block_line{static_cast<int>(pos+camera.get_x()), 0, 2, camera.get_height()};
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &block_line);
}
