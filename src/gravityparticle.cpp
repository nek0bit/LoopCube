#include "gravityparticle.hpp"

GravityParticle::GravityParticle(int texture_id, int time,
								 double start_vel_x, double start_vel_y, double x, double y, double width, double height)
	: Entity{texture_id, x, y, width, height}, time{0}, time_total{time} {
	vel_x = start_vel_x;
	vel_y = start_vel_y;
}

GravityParticle::~GravityParticle() {}

void GravityParticle::update(ChunkGroup& chunks) {
	time += 1;
	update_basic_physics(chunks);
}
	
bool GravityParticle::is_dead() const {
	return time > time_total; 
}
