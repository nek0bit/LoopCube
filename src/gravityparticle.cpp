#include "gravityparticle.hpp"

GravityParticle::GravityParticle(int modelId, int textureId, double time, double startVelX, double startVelY,
                                 double x, double y, double width, double height)
	: Entity{modelId, textureId, {x, y, 0}, {width, height}}, time{0}, timeTotal{time} {
	velX = startVelX;
	velY = startVelY;
    src.x = 4 * constants::blockImgSize;
}

void GravityParticle::update(ChunkGroup& chunks, const Timer& timer) {
	time += timer.deltaTime;
	updateBasicPhysics(chunks, timer);
}
