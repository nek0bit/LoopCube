#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <iostream>

#include "entity.hpp"
#include "position.hpp"
#include "collisioninfo.hpp"

class Player: public Entity {
public:
	Player();
	~Player();

	void update(ChunkGroup &chunks, std::vector<Entity*> entities);
	
	// Movement
	void jump(ChunkGroup &chunks);
	void direct_player(int direction, ChunkGroup &chunks);
private:
	// For jumping
	bool jumping;
	bool can_jump;
	bool jump_enabled;
};

#endif // PLAYER_HPP
