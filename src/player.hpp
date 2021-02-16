#pragma once
#include <iostream>
#include <memory>
#include <cmath>

#include "entity.hpp"
#include "collisioninfo.hpp"
#include "animation.hpp"
#include "spritesheet.hpp"

struct Player: public Entity
{
	Player();
	~Player();

	void update(ChunkGroup &chunks, Timer& timer, std::vector<std::shared_ptr<Entity>> entities);
	
	// Movement
	void jump(ChunkGroup &chunks, Timer& timer);
	void directPlayer(int direction, ChunkGroup &chunks, Timer& timer);
private:
	// Sprite stuff
	Spritesheet sprite;
	Animation frame;
	
	// For jumping
	bool jumping;
	bool canJump;
	bool jumpEnabled;
};
