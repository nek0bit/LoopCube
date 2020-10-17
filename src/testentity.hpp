#ifndef TESTENTITY_HPP
#define TESTENTITY_HPP
#include <iostream>

#include "entity.hpp"

class TestEntity: public Entity {
public:
    TestEntity() = default;
    TestEntity(TextureHandler &textures, double x, double y);
    ~TestEntity();

    void update(Chunk_Group &chunks, Camera& camera) override;
};

#endif
