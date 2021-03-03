#pragma once
#include <iostream>

#include "timer.hpp"
#include "chunkgroup.hpp"
#include "chunk.hpp"
#include "block.hpp"
#include "gameobj.hpp"
#include "collisioninfo.hpp"

struct Entity: public GameObject
{
	Entity() = default;
	Entity(int modelId, int textureId, glm::vec3 position, glm::vec2 size);
	virtual ~Entity();

    void update() override;
    virtual void update(ChunkGroup& chunks, const Timer& timer);

#ifndef __HEADLESS
    virtual void render(const Graphics& renderer, const Camera& camera)
        const override;
#endif
    void dummyInterpolate(const Timer& timer);


	// Collision functions
	virtual void collisionLeft();
	virtual void collisionRight();
	virtual void collisionBottom();
	virtual void collisionTop();

    double velX;
	double velY;
protected:
	void updateBasicPhysics(ChunkGroup& chunks, const Timer& timer);
	CollisionInfo checkBlockCollision(ChunkGroup &chunks);

    glm::vec3 displayPosition;

	// Physics
	double velXSpeed;
	bool onGround;
	int lastPos;
};
