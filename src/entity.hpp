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

class Entity: public GameObject {
public:
	Entity() = default;
	Entity(int textureId,
		   double x, double y, double width, double height);
	virtual ~Entity();

	virtual void update(ChunkGroup& chunks);


	// Collision functions
	virtual void collisionLeft();
	virtual void collisionRight();
	virtual void collisionBottom();
	virtual void collisionTop();

    double velX;
	double velY;
protected:
	void updateBasicPhysics(ChunkGroup& chunks);
	CollisionInfo checkBlockCollision(ChunkGroup &chunks);

	// Physics
	double vel_x_speed;
	bool onGround;
	int lastPos;
};

#endif
