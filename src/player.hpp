#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <iostream>

#include "entity.hpp"
#include "position.hpp"
#include "collisioninfo.hpp"

class Player: public Entity {
public:
    Player() = default;
    Player(TextureHandler &textures);
    ~Player();

    void update(Chunk_Group &chunks, Camera& camera, std::vector<Entity*> entities);
	
    // Movement
    void jump(Chunk_Group &chunks);
    void direct_player(int direction, Chunk_Group &chunks);
private:
	// For jumping
	bool jumping;
	bool can_jump;
	bool jump_enabled;
};

#endif // PLAYER_HPP
