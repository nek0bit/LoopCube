#include "chunk.hpp"

Chunk::Chunk(unsigned long int seed, int slot, std::vector<Structure*>& structures)
	: MAX_WIDTH{constants::chunk_width}, MAX_HEIGHT{constants::chunk_height}, MAX_SPLIT_COUNT{constants::chunk_split_count},
	  MAX_SPLIT_HEIGHT{constants::chunk_split_height}, terrain_gen{} {
	this->slot = slot;

	terrain_gen.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	terrain_gen.SetFractalType(FastNoiseLite::FractalType_FBm);
	terrain_gen.SetSeed(seed);
	terrain_gen.SetFractalOctaves(6);

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

Position Chunk::get_pos() const {
	return Position{slot*(MAX_WIDTH*constants::block_w), 0, (MAX_WIDTH*constants::block_w), 0};
}

int Chunk::get_chunk_x(int x) {
	return x+(slot*MAX_WIDTH);
}

int Chunk::get_chunk_y(int y) {
	int y_split = y / MAX_SPLIT_HEIGHT;
	return y - (y_split*MAX_SPLIT_HEIGHT);
}

int Chunk::get_y_split(double y) {
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
    // TODO use polymorphism for world generation for features such as multiple dimensions
	for (int x = 0; x < MAX_WIDTH; ++x) {
		double d_x = static_cast<double>(x)/static_cast<double>(MAX_WIDTH);

		// Generate world
		constexpr int compactness = 3; // I have no idea what I would've named these variables...
		constexpr int increase = 50;
		constexpr double cave_z = 1300;
		constexpr double cave_compactness_x = 5;
		constexpr double cave_compactness_y = 2400;
		constexpr double cave_increase = -.8;
		constexpr double index = 0.3;
		constexpr int offset = 160;
		constexpr int cave_start = 12;
		
	    int temp = floor( terrain_gen.GetNoise(static_cast<float>((d_x+slot))*compactness, static_cast<float>(0)) * increase );

		for (int y = 0; y < MAX_HEIGHT-temp-offset-MAX_SPLIT_HEIGHT; ++y) {
			double d_y = static_cast<double>(y)/static_cast<double>(MAX_HEIGHT);
			if (y == 0) {
				place_block_raw(BLOCK_GRASS, x, y+temp+offset);
				constexpr int tree_amount = 15;
			    int tree_generate = (terrain_gen.GetNoise(static_cast<double>(d_x*slot), d_y)*10)*tree_amount;
				if (x == 1) structures.push_back(new Tree(get_chunk_x(x), y+temp+offset-1));
			} else if (y >= 1 && y <= 3) {
				place_block_raw(BLOCK_DIRT, x, y+temp+offset);
			} else {
			    double cave_noise = terrain_gen.GetNoise((d_x+(slot))*cave_compactness_x, (d_y)*cave_compactness_y, cave_z)*cave_increase;
				if (y <= cave_start) {
					place_block_raw(BLOCK_STONE, x, y+temp+offset);
				} else if (cave_noise < index && cave_noise > index*-1) {
					place_block_raw(BLOCK_STONE, x, y+temp+offset);
				}
			}
		}
	}
}

const BlockInfo* Chunk::destroy_block(int x, int y, Inventory *inv) {
	int i = get_y_split(y);
	if (!(i < static_cast<int>(chunk.size()-1) && i >= 0)) {
		return nullptr;
	}
    // Search for block, destroy it, and add it to the inventory
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

// Similar to place_block_raw but does the checks for us, slow but safe
bool Chunk::place_block(int id, int x, int y) {
	// Check if between chunk size
	if (x < MAX_WIDTH+1 && x >= 0 && y < MAX_HEIGHT+1 && y >= 0) {
		// Check if a block has been placed here before
		bool is_duplicate = false;
		
		int i = get_y_split(y);
		if (!(i < static_cast<int>(chunk.size()-1) && i >= 0)) {
		    return false;
		}
		for (size_t j = 0; j < chunk[i].size(); ++j) {
			if (get_chunk_x(x)*constants::block_w == chunk[i][j].get_default_x() && y*constants::block_h == chunk[i][j].get_default_y()) {
				is_duplicate = true;
				break;
			}
		}
		
		if (!is_duplicate) {
			// It's safe to place the block			
			place_block_raw(id, x, y);
			
			return true;
		} else {
			return false;
		}
	}
	return false;
}

// Places a block in a position, unsafe but fast
void Chunk::place_block_raw(int id, int x, int y) {
	chunk[get_y_split(y)].push_back(Block{id, get_chunk_x(x), y});
}

void Chunk::update_all(Camera& camera) {
	render_all_functor(camera, [&](Block& blk) {
		blk.update();
	});
}


void Chunk::render_all_shadows(SDL_Renderer* renderer, Camera& camera) {
	render_all_functor(camera, [&](Block& blk) {
		blk.render_shadow(renderer, camera);
	});
}

void Chunk::render_all_functor(Camera& camera, std::function<void(Block&)> call) {
	const int offset = 10;
	const int flipped_camera = camera.get_y()*-1;
	const int chunk_pixel_height = constants::chunk_split_height*constants::block_h;
	
	for(size_t i = 0; i < chunk.size(); ++i) {
		const int min = get_y_split(flipped_camera - chunk_pixel_height - offset);
		const int max = get_y_split(flipped_camera + camera.get_height() + offset);
		const int check = get_y_split(i*chunk_pixel_height);
		
		for (size_t j = 0; j < chunk[i].size(); ++j) {
			if (!(min < check && max > check)) {
				break;
			}
			if (!chunk[i][j].out_of_view(camera)) {
				call(chunk[i][j]);
			}
		}
	}
}

void Chunk::render_all_blocks(SDL_Renderer* renderer, TextureHandler* textures, Camera& camera) {
	render_all_functor(camera, [&](Block& blk) {
                                   blk.render(renderer, textures, camera);
                               });
}

void Chunk::render_info(SDL_Renderer* renderer, Camera& camera) {
	int pos = get_chunk_x(0)*constants::block_w;
	/*if (chunk_text == nullptr) {
		SDL_Color color;
		color.r = 100; color.g = 100; color.b = 100; color.a = 255;
		// Note: chunk_text was nullptr before of course, so this is fine
		std::string tmp = std::string{"Chunk "} + std::to_string(get_slot());
		chunk_text = std::shared_ptr<Text>(new Text(renderer, tmp, color, constants::header_font));
	} else {
		chunk_text->draw(pos+20+camera.get_x(), 70);
	}*/

	// Draw gap
	SDL_Rect block_line{static_cast<int>(pos+camera.get_x()), 0, 2, camera.get_height()};
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &block_line);
}
