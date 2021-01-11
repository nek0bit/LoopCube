#pragma once
#include <iostream>
#include "entity.hpp"

class GravityParticle: public Entity {
public:
	GravityParticle() = default;
	GravityParticle(int textureId, int time, double startVelX, double startVelY,
                    double x, double y, double width = 10, double height = 10);
	~GravityParticle();
	
	void update(ChunkGroup &chunks) override;
	
	// Returns true if time reached
	// Appropriate to destruct or quit rendering when done.
	bool isDead() const;
private:
	int time;
	int timeTotal;
};
