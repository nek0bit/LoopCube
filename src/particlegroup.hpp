#pragma once
#include <vector>
#include "gravityparticle.hpp"
#include "instancedmodel.hpp"
#include "chunkgroup.hpp"
#include "timer.hpp"

struct ParticleGroup
{
    ParticleGroup();
    ~ParticleGroup();

    void addParticle(const GravityParticle& particle);
    void update(const ChunkGroup& chunks, const Timer& timer, InstancedModel& model) noexcept;
    void draw(const Graphics& graphics, const InstancedModel& model) const noexcept;
private:
    std::vector<GravityParticle> particles;
};
