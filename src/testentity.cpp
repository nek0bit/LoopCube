#include "testentity.hpp"

TestEntity::TestEntity(double x, double y)
	: Entity{0, x, y, 50, 50} {
	
}

TestEntity::~TestEntity() {

}

void TestEntity::update(ChunkGroup& chunks) {
	update_basic_physics(chunks);

	if (on_ground) {
		vel_y = -15;
	}
}

void TestEntity::collision_bottom() {
	vel_y = -5;
}
