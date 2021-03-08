#include "gravityparticle.hpp"

GravityParticle::GravityParticle(int modelId, int textureId, double time, double startVelX, double startVelY,
                                 double x, double y, double width, double height)
	: Entity{modelId, textureId, {x, y, 0}, {width, height}}, time{0}, timeTotal{time} {
	velX = startVelX;
	velY = startVelY;
    src.x = 4 * constants::blockImgSize;
}

void GravityParticle::update(const ChunkGroup& chunks, const Timer& timer) {
    constexpr int AMOUNT = 400;
	time += timer.deltaTime * AMOUNT;
	updateBasicPhysics(chunks, timer);
}
