#include "player.hpp"

Player::Player(TextureHandler &textures)
    : Game_Object{4, textures, 0, 0, 30, 58}, vel_x{0}, vel_y{0}, vel_x_speed{2} {
}

Player::~Player() {

}

bool Player::check_block_collision(Chunk_Group& chunks, Camera& camera) {
    std::vector<Chunk*>& chunkgroup = chunks.get_viewport_chunks();

	// I don't know why I have to do this but it works...
	//Chunk* c = chunks.get_chunk_at((obj.x/constants::block_w)-8, true); // Chunk in place
	//Chunk* c2 = chunks.get_chunk_at(((obj.x+obj.w)/constants::block_w)-8, true); // Chunk in front
	//Chunk* c3 = chunks.get_chunk_at(((obj.x-5-obj.w)/constants::block_w)-8, true); // Chunk in behind
    

	// Loop through all chunks
    for (auto *&chunk_it: chunkgroup) {

        // Then grab the chunk we want
        std::vector<Block>& chunk = chunk_it->get_chunk();

		//if (chunk_it->get_slot() == c->get_slot() || chunk_it->get_slot() == c2->get_slot() || chunk_it->get_slot() == c3->get_slot()) {
			// Loop through blocks in the chunk
			for (auto &block: chunk) {
				auto blockinfo = block.get_blockinfo();
				while (is_colliding(block, camera) && blockinfo->get_no_collision() != true) {
					return true;
				}
			}
		//}
    }
    return false;
}

void Player::jump(Chunk_Group &chunks, Camera& camera) {
    obj.y += 3;
    if (check_block_collision(chunks, camera)) {
        vel_y = -14;
    }
    obj.y -= 3;
}

void Player::direct_player(int direction, Chunk_Group &chunks, Camera& camera) {
    switch (direction) {
        case 0: // UP
            // TODO Optimize this, game checks for jumps repeatedly when held
            jump(chunks, camera);
            break;
        case 1: // RIGHT
            vel_x += vel_x_speed;
            last_pos = 1;
            break;
        case 2: // DOWN
            break;
        case 3: // LEFT
            vel_x -= vel_x_speed;
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
    vel_x *= 0.75;
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
    }

    vel_y += .7;
    obj.y += vel_y;

    // Check Y velocity
    if (check_block_collision(chunks, camera)) {
        obj.y += vel_y * -1;
        vel_y = 0;
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
