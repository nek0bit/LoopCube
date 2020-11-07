#include "chunkgroup.hpp"

// I could probably move the renderer and camera out of the class, but it doesn't take much memory so I think it's fine
ChunkGroup::ChunkGroup(unsigned long seed) : show_shadows{true}, render_chunk_info{true} {
	this->seed = seed;
	update_config();
}

ChunkGroup::~ChunkGroup() {

}

void ChunkGroup::update_config() {
	show_shadows = constants::config.get_int(CONFIG_SHOW_SHADOWS);
	render_chunk_info = constants::config.get_int(CONFIG_SHOW_CHUNK_DEBUG);
}

std::vector<Chunk>& ChunkGroup::get_chunks() {
	return group;
}

int ChunkGroup::get_chunk_y(double y) {
	int value = y / (constants::chunk_split_height * constants::block_h);
	if (!(constants::chunk_split_count-1 > value && value >= 0)) return -1;
	return value;
}

std::vector<Chunk*>& ChunkGroup::get_viewport_chunks() {
	return viewport_chunks;
}

bool ChunkGroup::chunk_already_generated(int id) {
	bool check = false;
	for (auto &i: past_chunks) {
		if (i == id) {
			check = true;
			break;
		}
	}
	return check;
}

void ChunkGroup::generate_chunk(int id, std::vector<Structure*>& structure) {
	bool check = chunk_already_generated(id);
	// Generate the chunk if it hasn't been generated before
	if (std::find(loaded_chunks.begin(), loaded_chunks.end(), id) == loaded_chunks.end() && !check) {
		Chunk temp_chunk(seed, id, structure);
		//chunk_async_loading(std::async(std::launch::async, build_chunk, seed, id, structures));
		
		insert_sorted(group, temp_chunk);
		insert_sorted(loaded_chunks, id);
		
		update_viewport();
	}

	// If chunk has been generated before, just reload an older chunk
	if (check) {
		// Get the index of the last unloaded chunk
		auto it_past_chunks = std::find(past_chunks.begin(), past_chunks.end(), id);
		int chunk_index = std::distance(past_chunks.begin(), it_past_chunks);
		try {
			// Add chunk back
			insert_sorted(group, group_past.at(chunk_index));
			insert_sorted(loaded_chunks, id);
			// Remove chunk from past_chunks to prevent from infinitely loaded chunks
			group_past.erase(group_past.begin() + chunk_index);
			past_chunks.erase(past_chunks.begin() + chunk_index);

		} catch (std::out_of_range &e) {
			std::cout << "[LOG] std::out_of_range handled: " << e.what() << std::endl;
		}
		update_viewport();
	}
}

void ChunkGroup::check_area(long long int x, std::vector<Structure*>& structures) {
	const int load_distance = constants::load_distance;
	
	// Takes x, and returns the current chunk id the player is in
	long int id = ceil(x / (constants::chunk_width * constants::block_w));

	// Unload old chunks
	// Warning: This code below is very fragile and can easily break :P
	if (static_cast<int>(loaded_chunks.size()) > (load_distance*2)+1) {
		/* We basically just move the chunk from group to group_past
		   Since we want to reload these chunks when the screen gets back in view */

		try {
			if (id-load_distance-1 < loaded_chunks.at(0)) {
				insert_sorted(group_past, *(group.end()-1));
				insert_sorted(past_chunks, *(loaded_chunks.end()-1));
				
				loaded_chunks.erase(loaded_chunks.end()-1);
				group.erase(group.end()-1);	
			}

			if (id+load_distance > loaded_chunks.at(loaded_chunks.size()-1)) {
				insert_sorted(group_past, group[0]);
				insert_sorted(past_chunks, loaded_chunks[0]);
				
				loaded_chunks.erase(loaded_chunks.begin());
				group.erase(group.begin());
			}
		
			update_viewport();
		} catch(std::out_of_range& err) {
			std::cout << "Out of bounds..." << std::endl;
		}
	}

	// TODO Only run generate_chunk if needed
	for (int i = load_distance*-1; i < load_distance; ++i) {
		generate_chunk(id+i, structures);
	}
}

// This function is just a lazy attempt at debugging
void ChunkGroup::print_chunks_fancy() {
	std::cout << "=========================================================" << std::endl;
	// Used for debugging sometimes
	// loaded_chunks
	std::cout << "group: ";
	for (auto &i: group) {
		std::cout << i.get_slot() << ", ";
	}
	std::cout << std::endl;
	std::cout << "loaded_chunks: ";
	for (auto &i: loaded_chunks) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;
	
	// past_chunks
	std::cout << "group_past: ";
	for (auto &j: group_past) {
		std::cout << j.get_slot() << ", ";
	}
	std::cout << std::endl;
	std::cout << "past_chunks: ";
	for (auto &j: past_chunks) {
		std::cout << j << ", ";
	}
	std::cout << std::endl;
	std::cout << "Size[loaded_chunks]: " << group.size() << std::endl;
	std::cout << "Size[past_chunks]: " << group_past.size() << std::endl;
}

void ChunkGroup::update_viewport() {
	const int total_load_dist = constants::load_distance * 2;
	const int load_dist_midpoint = (total_load_dist / 2) - constants::load_viewport / 2;
	
	// Populate viewport_chunks with references
	viewport_chunks.clear();
	for (int i = 0; i < constants::load_viewport; ++i) {
		Chunk* temp = &group[i+load_dist_midpoint];
		viewport_chunks.push_back(temp);
	}
}

// Deprecated Usage: Using sort_all is slow, now we manually sort chunks on the fly
void ChunkGroup::sort_all() {
	std::sort(loaded_chunks.begin(), loaded_chunks.end());
	std::sort(group.begin(), group.end());
	std::sort(group_past.begin(), group_past.end());
	std::sort(past_chunks.begin(), past_chunks.end());
}

Chunk* ChunkGroup::get_chunk_at(double x, bool loaded=true) {
	double id_double;
	if (x >= 0) {
		id_double = x / (constants::chunk_width * constants::block_w);
	} else {
		id_double = floor(x / (constants::chunk_width * constants::block_w));
	}

	int id = id_double;	
	std::vector<int>::iterator hovered_chunk = std::find(loaded_chunks.begin(), loaded_chunks.end(), id);

	try {
		if (hovered_chunk != loaded_chunks.end()) {
			int chunk_index = std::distance(loaded_chunks.begin(), hovered_chunk);
			return &group.at(chunk_index);
		}
	
		// Failed to find chunk in loaded chunks, lets search for past chunks if requested
		if (!loaded) {
			std::vector<int>::iterator hovered_chunk_past = std::find(past_chunks.begin(), past_chunks.end(), id);

			if (hovered_chunk_past != past_chunks.end()) {
				int chunk_index = std::distance(past_chunks.begin(), hovered_chunk_past);
				return &group_past.at(chunk_index);
			}
		}
	} catch(std::out_of_range& err) { return nullptr; }
	
	return nullptr;
}

int ChunkGroup::get_id(Camera& camera, int surrounding) {
	double id = 0;
	id = ceil((camera.get_x() - (camera.get_width()/2))	 / (surrounding * constants::block_w));
	if (id > 0) {
		id *= -1;
	} else {
		id = abs(id);
	}
	return id;
}

void ChunkGroup::render_all_viewport(GraphicsWrapper* renderer, Camera& camera) {
	if (show_shadows) {
		for (auto &chunk: viewport_chunks) {
			Position box = chunk->get_pos();
			if ((box.x+box.w)+camera.get_x() < 0 ||
				camera.get_width() < ((box.x+box.w)+camera.get_x())-(constants::chunk_width*constants::block_w)) {
				continue;
			} else {
				chunk->render_all_shadows(renderer, camera);
			}
		}
	}
	for (auto &chunk: viewport_chunks) {
		Position box = chunk->get_pos();
		if ((box.x+box.w)+camera.get_x() < 0 ||
			camera.get_width() < ((box.x+box.w)+camera.get_x())-(constants::chunk_width*constants::block_w)) {
			continue;
		} else {
			chunk->render_all_blocks(renderer, camera);
		}
	}
	if (render_chunk_info) {
		for (auto &chunk: viewport_chunks) { 
			chunk->render_info(renderer, camera);
		}
	}
}

void ChunkGroup::update_all_viewport(Camera& camera) {
	for (auto &chunk: viewport_chunks) {
		Position box = chunk->get_pos();
		if ((box.x+box.w)+camera.get_x() < 0 ||
			camera.get_width() < ((box.x+box.w)+camera.get_x())-(constants::chunk_width*constants::block_w)) {
			continue;
		} else {
			chunk->update_all(camera);
		}
	}
}
