#pragma once

#include "entity.hpp"

struct GravityParticle: public Entity
{
	GravityParticle() = default;
	GravityParticle(int modelId, int textureId, double time, double startVelX, double startVelY,
                    double x, double y, double width = 10, double height = 10);
	~GravityParticle() = default;
	
	void update(ChunkGroup &chunks, const Timer& timer) override;
	
	// Returns true if time reached
	// Appropriate to destruct or quit rendering when done.
	inline bool isDead() const
        {
            return time > timeTotal;
        }
private:
    double time;
    double timeTotal;
};
