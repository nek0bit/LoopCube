#include "player.hpp"

enum Sprites {
	PLAYER_IDLE,
	PLAYER_WALK_CYCLE1,
	PLAYER_WALK_CYCLE2,
	PLAYER_WALK_CYCLE3,
	PLAYER_WALK_CYCLE4,
	PLAYER_JUMP
};

Player::Player()
	: Entity{4, 0, 130*constants::block_h, 34, 66}, sprite{17, 33}, frame{40}, jumping{false}, can_jump{true}, jump_enabled{true}  {
	
}

Player::~Player() {

}

void Player::update(ChunkGroup& chunks, std::vector<Entity*> entities) {
	update_basic_physics(chunks);
	frame.tick();

	// Frame for when the character is looking the other way
	const int max_frames = 5;
	int flipped = max_frames;

	int on_frame = ceil((static_cast<double>(frame.get_frame())/static_cast<double>(frame.get_max_frames())
						 )*(max_frames-1));
	if (on_frame == 0) on_frame = 1;

	// Where the player is looking
	static int adder = 0;
	
	if ((vel_x < -0.1 || vel_x > 0.1) && on_ground) {
		if (vel_x < -0.1) {
			// Walking left
			sprite.set_x(on_frame + flipped);
			adder = flipped;
		} else if (vel_x > 0.1) {
			// Walking right
			sprite.set_x(on_frame);
			adder = 0;
		}
	} else if (jumping) {
		// Jumping
		sprite.set_x(PLAYER_IDLE + adder);
	} else {
		// Idle
		sprite.set_x(PLAYER_IDLE + adder);
	}

	src.x = sprite.get_x();
	src.y = sprite.get_y();
	src.w = sprite.get_width();
	src.h = sprite.get_height();

	// See if touching entities
	for (auto*& entity: entities) {
		CollisionInfo info = is_colliding(*entity);
		if (info.colliding) {
			if (info.bottom >= 0) {
				vel_y = 0;
				entity->collision_bottom();
				entity->update(chunks);
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

void Player::jump(ChunkGroup &chunks) {	
	obj.y += 1;
	if (on_ground && jump_enabled && check_block_collision(chunks).top != -1) {
		vel_y = -12;
		on_ground = false;
	}
	obj.y -= 1;
	jumping = true;
	can_jump = false;
}

void Player::direct_player(int direction, ChunkGroup &chunks) {
	switch (direction) {
	case 0: // UP
		jump(chunks);
		break;
	case 1: // RIGHT
		if (!on_ground) vel_x += vel_x_speed/3;
		else vel_x += vel_x_speed;
		last_pos = 1;
		break;
	case 2: // DOWN
		break;
	case 3: // LEFT
		if (!on_ground) vel_x -= vel_x_speed/3;
		else vel_x -= vel_x_speed;
		last_pos = 3;
		break;
	default:
		std::cerr << "[Warning] Invalid direction" << std::endl;
	}
}
