#include "player.hpp"

Player::Player(TextureHandler &textures)
    : Entity{textures, 4, 0, 0, 30, 58}, jumping{false}, can_jump{true}, jump_enabled{true} {
	
}

Player::~Player() {

}

void Player::update(Chunk_Group& chunks, Camera& camera, std::vector<Entity*> entities) {
	update_basic_physics(chunks, camera);

	// See if touching entities
	for (auto*& entity: entities) {
	    CollisionInfo info = is_colliding(*entity);
		if (info.colliding) {
			if (info.bottom >= 0) {
				vel_y = 0;
				entity->collision_bottom();
				entity->update(chunks, camera);
			}
			if (info.top >= 0) {
				obj.y -= info.top;
				vel_y = 0;
			}
		}
	}
	
	// Prevent player from holding jump
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

	// Reset
	jumping = false;
	can_jump = true;
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
			jump(chunks);
            break;
        case 1: // RIGHT
			if (!on_ground) vel_x += vel_x_speed/6;
			else vel_x += vel_x_speed;
			last_pos = 1;
            break;
        case 2: // DOWN
            break;
        case 3: // LEFT
			if (!on_ground) vel_x -= vel_x_speed/6;
			else vel_x -= vel_x_speed;
			last_pos = 3;
            break;
        default:
            std::cerr << "[Warning] Invalid direction" << std::endl;
    }
}
