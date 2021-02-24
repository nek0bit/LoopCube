#include "player.hpp"

enum Sprites
{
	PLAYER_IDLE,
	PLAYER_WALK_CYCLE1,
	PLAYER_WALK_CYCLE2,
	PLAYER_WALK_CYCLE3,
	PLAYER_WALK_CYCLE4,
	PLAYER_JUMP
};

Player::Player()
	: Entity{TEXTURE_PLAYER, 0, 0, 34, 66},
      sprite{17, 33},
      frame{40},
      jumping{false},
      canJump{true},
      jumpEnabled{true}
{}

Player::~Player()
{}

void Player::update(ChunkGroup& chunks, Timer& timer,
                    std::vector<std::shared_ptr<Entity>> entities, bool interpolate)
{
    if (!interpolate) updateBasicPhysics(chunks, timer);
    else dummyInterpolate(timer);
	frame.tick(timer, 50);

	// Frame for when the character is looking the other way
	constexpr int maxFrames = 5;

	int onFrame = ceil(static_cast<double>(frame.frame) / static_cast<double>(frame.maxFrames)
                       *(maxFrames-1));
	if (onFrame == 0) onFrame = 1;

	// Where the player is looking
	static float adder = 0;
	
	if ((velX < -0.1 || velX > 0.1) && onGround)
    {
		if (velX < -0.1)
        {
			// Walking left
			sprite.x = onFrame + maxFrames;
			adder = maxFrames;
		}
        else if (velX > 0.1)
        {
			// Walking right
			sprite.x = onFrame;
			adder = 0;
		}
	}
    else if (jumping)
    {
		// Jumping
		sprite.x = PLAYER_IDLE + adder;
	}
    else
    {
		// Idle
		sprite.x = PLAYER_IDLE + adder;
	}

	src.x = sprite.getX();
	src.y = sprite.getY();
	src.w = sprite.width;
	src.h = sprite.height;

	// See if touching entities
	for (auto& entity: entities)
    {
		CollisionInfo info = isColliding(*entity);
		if (info.colliding)
        {
			if (info.bottom >= 0)
            {
				velY = 0;
				entity->collisionBottom();
				entity->update(chunks, timer);
			}
			if (info.top >= 0)
            {
				position.y -= info.top;
				velY = 0;
			}
		}
	}
	
	// Prevent player from holding jump
	if (!canJump && onGround)
    {
		jumpEnabled = false;
	}
		
	if (canJump && onGround)
    {
		jumpEnabled = true;
	}
	
	// Check if jump released
	if (!onGround && !jumping)
    {
		if (velY < -20.0f) {
			velY += 30.06f;
		}
	}

	// Reset
	jumping = false;
	canJump = true;
}

void Player::jump(ChunkGroup &chunks, Timer&)
{
	position.y += 1;
	if (onGround && jumpEnabled && checkBlockCollision(chunks).top != -1) {
        velY = -885.0f;
        onGround = false;
    }
	position.y -= 1;
	jumping = true;
	canJump = false;
}

void Player::directPlayer(int direction, ChunkGroup &chunks, Timer& timer)
{
	switch (direction)
    {
	case 0: // UP
		jump(chunks, timer);
		break;
	case 1: // RIGHT
        velX += velXSpeed * timer.deltaTime;
		lastPos = 1;
		break;
	case 2: // DOWN
		break;
	case 3: // LEFT
        velX -= velXSpeed * timer.deltaTime;
		lastPos = 3;
		break;
	default:
        break;
	}
}
