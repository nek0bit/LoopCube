#include "testentity.hpp"

TestEntity::TestEntity(double x, double y)
	: Entity{0, x, y, 50, 50} {
	
}

TestEntity::~TestEntity() {

}

void TestEntity::update(ChunkGroup& chunks, Timer& timer) {
	updateBasicPhysics(chunks, timer);

	if (onGround) {
		velY = -15;
	}
}

void TestEntity::collisionBottom() {
	velY = -5;
}
