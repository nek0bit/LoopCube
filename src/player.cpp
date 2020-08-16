#include "player.hpp"

Player::Player(TextureHandler &textures)
    : Game_Object{4, textures, 0, 0, 30, 58}, vel_x{0}, vel_y{0}, vel_x_speed{2} {
}

Player::~Player() {

}

// TODO optimize this!!! I'm wasting CPU time by copying the chunks!!!
bool Player::check_block_collision(Camera& camera, Chunk_Group &chunks) {
    std::vector<Chunk> chunkgroup = *chunks.get_chunks();

    // Loop through all chunks
    for (auto &chunk_it: chunkgroup) {

        // Then grab the chunk we want
        std::vector<Block> chunk = *chunk_it.get_chunk();

        // Loop through blocks in the chunk
        for (auto &block: chunk) {
            while (is_colliding(camera, block)) {
                return true;
            }
        }
    }
    return false;
}

void Player::jump(Camera& camera, Chunk_Group &chunks) {
    obj.y += 3;
    if (check_block_collision(camera, chunks)) {
        vel_y = -14;
    }
    obj.y -= 3;
}

void Player::direct_player(Camera& camera, int direction, Chunk_Group &chunks) {
    switch (direction) {
        case 0: // UP
            // TODO Optimize this, game checks for jumps repeatedly when held
            jump(camera, chunks);
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

void Player::update(Camera& camera, Chunk_Group &chunks) {
    // TODO move engine code into it's own class for reusability
    vel_x *= 0.75;
    obj.x += vel_x;



    // Check X velocity
    if (check_block_collision(camera, chunks)) {
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
    if (check_block_collision(camera, chunks)) {
        obj.y += vel_y * -1;
        vel_y = 0;
    }


    // Update draw position
    src.h = get_height();
    src.w = get_width();
    src.x = 0;
    src.y = 0;

    obj.x = get_x(camera);
    obj.y = get_y(camera);
}
