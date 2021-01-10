#pragma once
#include <iostream>
#include "entity.hpp"

class GravityParticle: public Entity {
public:
	GravityParticle() = default;
	GravityParticle(int texture_id, int time,
					double start_vel_x, double start_vel_y, double x, double y, double width = 10, double height = 10);
	~GravityParticle();
	
	void update(ChunkGroup &chunks) override;
	
	// Returns true if time reached
	// Appropriate to destruct or quit rendering when done.
	bool is_dead() const;
private:
	int time;
	int time_total;
};
