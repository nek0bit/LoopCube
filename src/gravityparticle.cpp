#include "gravityparticle.hpp"

GravityParticle::GravityParticle(int textureId, double time, double startVelX, double startVelY,
                                 double x, double y, double width, double height)
	: Entity{textureId, x, y, width, height}, time{0}, timeTotal{time} {
	velX = startVelX;
	velY = startVelY;
}

void GravityParticle::update(ChunkGroup& chunks, Timer& timer) {
	time += timer.deltaTime;
	updateBasicPhysics(chunks, timer);
}
	
bool GravityParticle::isDead() const {
	return time > timeTotal; 
}
