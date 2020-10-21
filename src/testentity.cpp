#include "testentity.hpp"

TestEntity::TestEntity(TextureHandler &textures, double x, double y)
	: Entity{textures, 0, x, y, 50, 50} {
	
}

TestEntity::~TestEntity() {

}

void TestEntity::update(ChunkGroup& chunks, Camera& camera) {
	update_basic_physics(chunks, camera);

	if (on_ground) {
		vel_y = -15;
	}
}

void TestEntity::collision_bottom() {
	vel_y = -5;
}
