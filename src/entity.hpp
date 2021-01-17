#pragma once
#include <iostream>

#include <SDL2/SDL.h>

#include "timer.hpp"
#include "chunkgroup.hpp"
#include "camera.hpp"
#include "chunk.hpp"
#include "block.hpp"
#include "gameobj.hpp"
#include "collisioninfo.hpp"

struct Entity: public GameObject
{
	Entity() = default;
	Entity(int textureId,
		   double x, double y, double width, double height);
	virtual ~Entity();

	virtual void update(ChunkGroup& chunks, Timer& timer);


	// Collision functions
	virtual void collisionLeft();
	virtual void collisionRight();
	virtual void collisionBottom();
	virtual void collisionTop();

    double velX;
	double velY;
protected:
	void updateBasicPhysics(ChunkGroup& chunks, Timer& timer);
	CollisionInfo checkBlockCollision(ChunkGroup &chunks);

	// Physics
	double velXSpeed;
	bool onGround;
	int lastPos;
};
