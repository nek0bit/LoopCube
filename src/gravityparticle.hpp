#ifndef GRAVITYPARTICLE_HPP
#define GRAVITYPARTICLE_HPP
#include <iostream>
#include "entity.hpp"
#include "aabb.hpp"

class GravityParticle: public Entity {
public:
	GravityParticle() = default;
	GravityParticle(int texture_id, TextureHandler& textures, int time,
					double start_vel_x, double start_vel_y, double x, double y, double width = 10, double height = 10);
	~GravityParticle();
	
	void update(ChunkGroup &chunks, Camera& camera) override;
	
	// Returns true if time reached
	// Appropriate to destruct or quit rendering when done.
	bool is_dead() const;
private:
	int time;
	int time_total;
};


#endif // GRAVITYPARTICLE_HPP
