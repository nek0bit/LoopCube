#include "player.hpp"

Player::Player(TextureHandler &textures)
    : Game_Object{4, textures, 0, 0, 30, 58}, free_move_debug{false}, vel_x{0}, vel_y{0}, vel_x_speed{1.8}, on_ground{false}, jumping{false}, can_jump{true}, jump_enabled{true} {
	
}

Player::~Player() {

}

// TODO Check if certain block is higher or lower then player
CollisionInfo Player::check_block_collision(Chunk_Group& chunks) {
    std::vector<Chunk*>& chunkgroup = chunks.get_viewport_chunks();

	Chunk* c_behind = chunks.get_chunk_at(obj.x, true);
	Chunk* c_front = chunks.get_chunk_at(obj.x+obj.w, true);

	// Loop through all chunks
    for (auto *&chunk_it: chunkgroup) {
		// See if player is within this chunk, if so, move on and handle collision
		// If not, it's pointless to check the chunk, continue
		if (chunk_it->get_slot() == c_behind->get_slot() ||
			chunk_it->get_slot() == c_front->get_slot()) {

			// Store chunk as reference for further usage
			std::vector<Block>& chunk = chunk_it->get_chunk();
			// Loop through blocks in chunk
			for (auto &block: chunk) {
				auto blockinfo = block.get_blockinfo();
				CollisionInfo info = is_colliding(block);
				while (info == true && blockinfo->get_no_collision() != true) {
					return info;
				}
			}
		}
    }
    return CollisionInfo{};
}

void Player::jump(Chunk_Group &chunks) {	
	obj.y += 1;
    if (on_ground && jump_enabled && check_block_collision(chunks).top != -1) {
        vel_y = -12;
		on_ground = false;
    }
    obj.y -= 1;
	jumping = true;
	can_jump = false;
}

void Player::direct_player(int direction, Chunk_Group &chunks) {
    switch (direction) {
        case 0: // UP
			if (!free_move_debug) {
				jump(chunks);
			} else {
				obj.y -= 5;
			}
            break;
        case 1: // RIGHT
			if (!free_move_debug) {
				if (!on_ground) vel_x += vel_x_speed/6;
				else vel_x += vel_x_speed;
				last_pos = 1;
			} else {
				obj.x += 5;
			}
            break;
        case 2: // DOWN
			if (free_move_debug) {
				obj.y += 5;
			}
            break;
        case 3: // LEFT
			if (!free_move_debug) {
				if (!on_ground) vel_x -= vel_x_speed/6;
				else vel_x -= vel_x_speed;
				last_pos = 3;
			} else {
				obj.x -= 5;
			}
            break;
        default:
            std::cerr << "[Warning] Invalid direction" << std::endl;
    }
}

double Player::get_vel_x() const {
    return vel_x;
}

double Player::get_vel_y() const {
    return vel_y;
}

double Player::get_x(Camera& camera) const {
	return obj.x + (camera.get_x());
}

double Player::get_y(Camera& camera) const {
    return obj.y + (camera.get_y());
}

void Player::update(Chunk_Group &chunks, Camera& camera) {
    // TODO move engine code into it's own class for reusability

	//if (free_move_debug) check_block_collision(chunks);
	
	if (!free_move_debug) {
		if (on_ground) vel_x *= 0.78;

		// Reset back
		if (vel_x > 6) vel_x = 6;
		if (vel_x < -6) vel_x = -6;
		
		obj.x += vel_x;

		CollisionInfo info_x = check_block_collision(chunks);

		// Check X velocity
		if (info_x == true) {
			if (vel_x == 0) {
				// If player happens to get stuck in the wall then push them out
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
		
		if (!can_jump && on_ground) {
			jump_enabled = false;
		}
		
		if (can_jump && on_ground) {
			jump_enabled = true;
		}
		
		
		// Check if jump released
		if (!on_ground && !jumping) {
			if (vel_y < -5) {
				vel_y += 1.0;
			}
		}
		
		jumping = false;
		can_jump = true;
	}

    // Update draw position
    src.h = get_height();
    src.w = get_width();
    src.x = 0;
    src.y = 0;

    dest.h = src.h;
    dest.w = src.w;
    dest.x = get_x(camera);
    dest.y = get_y(camera);
}
