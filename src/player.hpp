#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <iostream>
#include <cmath>

#include "entity.hpp"
#include "position.hpp"
#include "collisioninfo.hpp"
#include "animation.hpp"
#include "spritesheet.hpp"

class Player: public Entity {
public:
	Player();
	~Player();

	void update(ChunkGroup &chunks, std::vector<Entity*> entities);
	
	// Movement
	void jump(ChunkGroup &chunks);
	void direct_player(int direction, ChunkGroup &chunks);
private:
	// Sprite stuff
	Spritesheet sprite;
	Animation frame;
	
	// For jumping
	bool jumping;
	bool canJump;
	bool jumpEnabled;
};

#endif // PLAYER_HPP
