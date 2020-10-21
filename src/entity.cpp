#include "entity.hpp"

Entity::Entity(TextureHandler& textures, int texture_id, double x, double y, double width, double height)
	: Game_Object{texture_id, textures, x, y, width, height}, vel_x{0}, vel_y{0}, vel_x_speed{1.8}, on_ground{false}, last_pos{-1} {
	
}

Entity::~Entity() {}

void Entity::update(Chunk_Group& chunks, Camera& camera) {
	// Optional; You can use your own physics function
	update_basic_physics(chunks, camera);
}

double Entity::get_vel_x() const {
    return vel_x;
}

double Entity::get_vel_y() const {
    return vel_y;
}

double Entity::get_x(Camera& camera) const {
	return obj.x + (camera.get_x());
}

double Entity::get_y(Camera& camera) const {
    return obj.y + (camera.get_y());
}

void Entity::collision_left() {}
void Entity::collision_right() {}
void Entity::collision_bottom() {}
void Entity::collision_top() {}

CollisionInfo Entity::check_block_collision(Chunk_Group& chunks) {
    std::vector<Chunk*>& chunkgroup = chunks.get_viewport_chunks();

	Chunk* c = chunks.get_chunk_at(obj.x, true);
	Chunk* c_front = chunks.get_chunk_at(obj.x+obj.w, true);

	// Loop through all chunks
    for (auto *&chunk_it: chunkgroup) {
		// See if player is within this chunk, if so, move on and handle collision
		// If not, it's pointless to check the chunk, continue
		if (chunk_it->get_slot() == c->get_slot() ||
			chunk_it->get_slot() == c_front->get_slot()) {

			// Store chunk as reference for further usage
			std::vector<std::vector<Block>>& chunk = chunk_it->get_chunk();

			std::array<int, 2> player_check = {
				chunks.get_chunk_y(obj.y),
				chunks.get_chunk_y(obj.y+obj.h)
			};
			
			
			for (auto &i: player_check) { 
				for (auto &block: chunk[i]) {
					auto blockinfo = block.get_blockinfo();
					CollisionInfo info = is_colliding(block);
					while (info == true && blockinfo->get_no_collision() != true) {
						return info;
					}
				}
			}
		}
    }
    return CollisionInfo{};
}

void Entity::update_basic_physics(Chunk_Group& chunks, Camera& camera) {
	// Update draw position
    src.h = get_height();
    src.w = get_width();
    src.x = 0;
    src.y = 0;

    dest.h = src.h;
    dest.w = src.w;
    dest.x = get_x(camera);
    dest.y = get_y(camera);
	
	if (on_ground) vel_x *= 0.78;

	// Reset back
	if (vel_x > 6) vel_x = 6;
	if (vel_x < -6) vel_x = -6;
		
	obj.x += vel_x;

	CollisionInfo info_x = check_block_collision(chunks);

	// Check X velocity
	if (info_x == true) {
		if (vel_x == 0) {
			// If entity happens to get stuck in the wall then push them out
			if (last_pos == 1) {
				obj.x -= 5;
			} else if (last_pos == 3) {
				obj.x += 5;
			} else {
				obj.x += 5;
			}
				
		}
		if (info_x.left != -1) {
			obj.x -= info_x.left;
		}
		if (info_x.right != -1) {
			obj.x += info_x.right;
		}
		vel_x = 0;
		on_ground = true;
	}
		
	vel_y += .5;
	obj.y += vel_y;

	CollisionInfo info_y = check_block_collision(chunks);
		
	// Check Y velocity
	if (info_y == true) {
		if (info_y.bottom != -1) {
			obj.y += info_y.bottom;
		}
		if (info_y.top != -1) {
			obj.y -= info_y.top;
		}
		vel_y = 0;
		on_ground = true;
	} else {
		on_ground = false;
	}
}
