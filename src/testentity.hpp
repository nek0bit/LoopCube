#ifndef TESTENTITY_HPP
#define TESTENTITY_HPP
#include <iostream>

#include "entity.hpp"

class TestEntity: public Entity {
public:
	TestEntity() = default;
	TestEntity(double x, double y);
	~TestEntity();

	void update(ChunkGroup &chunks, Timer& timer) override;

	void collisionBottom() override;
};

#endif
