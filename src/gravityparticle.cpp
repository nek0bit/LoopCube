#include "gravityparticle.hpp"

GravityParticle::GravityParticle(int textureId, int time, double startVelX, double startVelY,
                                 double x, double y, double width, double height)
	: Entity{textureId, x, y, width, height}, time{0}, timeTotal{time} {
	velX = startVelX;
	velY = startVelY;
}

void GravityParticle::update(ChunkGroup& chunks) {
	time++;
	updateBasicPhysics(chunks);
}
	
bool GravityParticle::isDead() const {
	return time > timeTotal; 
}
