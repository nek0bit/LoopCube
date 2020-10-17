#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <iostream>

#include <SDL2/SDL.h>

#include "chunkgroup.hpp"
#include "camera.hpp"
#include "chunk.hpp"
#include "block.hpp"
#include "gameobj.hpp"
#include "collisioninfo.hpp"

class Entity: public Game_Object {
public:
	Entity() = default;
    Entity(TextureHandler& textures, int texture_id,
		   double x, double y, double width, double height);
	virtual ~Entity();

	virtual void update(Chunk_Group& chunks, Camera& camera);

	double get_vel_x() const;
    double get_vel_y() const;
	double get_x(Camera& camera) const;
	double get_y(Camera& camera) const;
protected:
	void update_basic_physics(Chunk_Group& chunks, Camera& camera);
	CollisionInfo check_block_collision(Chunk_Group &chunks);

	// Physics
	double vel_x;
	double vel_y;
	double vel_x_speed;
	bool on_ground;
	int last_pos;
};

#endif
