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
	: Entity{4, 0, 130*constants::blockH, 34, 66},
      sprite{17, 33},
      frame{40},
      jumping{false},
      canJump{true},
      jumpEnabled{true}
{}

Player::~Player()
{}

void Player::update(ChunkGroup& chunks, Timer& timer, std::vector<Entity*> entities)
{
	updateBasicPhysics(chunks, timer);
	frame.tick();

	// Frame for when the character is looking the other way
	constexpr int maxFrames = 5;

	int onFrame = ceil((static_cast<double>(frame.frame)/static_cast<double>(frame.maxFrames)
						 )*(maxFrames-1));
	if (onFrame == 0) onFrame = 1;

	// Where the player is looking
	static int adder = 0;
	
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
	for (auto*& entity: entities)
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
		if (velY < -.42) {
			velY += 0.06;
		}
	}

	// Reset
	jumping = false;
	canJump = true;
}

void Player::jump(ChunkGroup &chunks, Timer& timer)
{
	position.y += 1;
	if (onGround && jumpEnabled && checkBlockCollision(chunks).top != -1) {
		velY = -.85;
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
        if (!onGround) velX += (velXSpeed*0.02) * timer.deltaTime.ms;
        velX += velXSpeed * timer.deltaTime.ms;
		lastPos = 1;
		break;
	case 2: // DOWN
		break;
	case 3: // LEFT
        if (!onGround) velX -= (velXSpeed*0.02) * timer.deltaTime.ms;
        velX -= velXSpeed * timer.deltaTime.ms;
		lastPos = 3;
		break;
	default:
        break;
	}
}
