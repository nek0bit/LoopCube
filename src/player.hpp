#pragma once
#include <iostream>
#include <cmath>

#include "entity.hpp"
#include "position.hpp"
#include "collisioninfo.hpp"
#include "animation.hpp"
#include "spritesheet.hpp"

struct Player: public Entity
{
	Player();
	~Player();

	void update(ChunkGroup &chunks, std::vector<Entity*> entities);
	
	// Movement
	void jump(ChunkGroup &chunks);
	void directPlayer(int direction, ChunkGroup &chunks);
private:
	// Sprite stuff
	Spritesheet sprite;
	Animation frame;
	
	// For jumping
	bool jumping;
	bool canJump;
	bool jumpEnabled;
};
