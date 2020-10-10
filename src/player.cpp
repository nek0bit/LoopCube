#include "player.hpp"

Player::Player(TextureHandler &textures)
    : Game_Object{4, textures, 0, 0, 30, 58}, vel_x{0}, vel_y{0}, vel_x_speed{1.8}, on_ground{false}, jumping{false}, can_jump{true}, jump_enabled{true} {
	
}

Player::~Player() {

}

bool Player::check_block_collision(Chunk_Group& chunks, Camera& camera) {
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
				while (is_colliding(block, camera) && blockinfo->get_no_collision() != true) {
					return true;
				}
			}
		}
    }
    return false;
}

void Player::jump(Chunk_Group &chunks, Camera& camera) {	
	obj.y += 1;
    if (on_ground && jump_enabled) {
        vel_y = -12;
		on_ground = false;
    }
    obj.y -= 1;
	jumping = true;
	can_jump = false;
}

void Player::direct_player(int direction, Chunk_Group &chunks, Camera& camera) {
    switch (direction) {
        case 0: // UP
            jump(chunks, camera);
            break;
        case 1: // RIGHT
            if (!on_ground) vel_x += vel_x_speed/12;
			else vel_x += vel_x_speed;
            last_pos = 1;
            break;
        case 2: // DOWN
            break;
        case 3: // LEFT
            if (!on_ground) vel_x -= vel_x_speed/12;
			else vel_x -= vel_x_speed;
            last_pos = 3;
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

void Player::update(Chunk_Group &chunks, Camera& camera) {
    // TODO move engine code into it's own class for reusability
	
	if (on_ground) vel_x *= 0.78;

	// Reset back
    if (vel_x > 6) vel_x = 6;
	if (vel_x < -6) vel_x = -6;
	
	obj.x += vel_x;


    // Check X velocity
    if (check_block_collision(chunks, camera)) {
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
        obj.x += vel_x * -1;
        vel_x = 0;
		on_ground = true;
    }

    vel_y += .5;
    obj.y += vel_y;

    // Check Y velocity
    if (check_block_collision(chunks, camera)) {
        obj.y += vel_y * -1;
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
