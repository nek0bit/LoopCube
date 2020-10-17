#include "testentity.hpp"

TestEntity::TestEntity(TextureHandler &textures, double x, double y)
    : Entity{textures, 0, x, y, 50, 50} {
	
}

TestEntity::~TestEntity() {

}

void TestEntity::update(Chunk_Group& chunks, Camera& camera) {
	update_basic_physics(chunks, camera);

	if (on_ground) {
		vel_y = -15;
	}
}
