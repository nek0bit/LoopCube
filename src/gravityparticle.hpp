#pragma once

#include "entity.hpp"

struct GravityParticle: public Entity
{
	GravityParticle() = default;
	GravityParticle(int textureId, double time, double startVelX, double startVelY,
                    double x, double y, double width = 10, double height = 10);
	~GravityParticle() = default;
	
	void update(ChunkGroup &chunks, Timer& timer) override;
	
	// Returns true if time reached
	// Appropriate to destruct or quit rendering when done.
	bool isDead() const;
private:
    float time;
    float timeTotal;
};
