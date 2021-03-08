#include "particlegroup.hpp"

ParticleGroup::ParticleGroup()
    : particles{}
{
    
}

ParticleGroup::~ParticleGroup()
{}

void ParticleGroup::addParticle(const GravityParticle& particle)
{
    particles.emplace_back(particle);
}

void ParticleGroup::update(const ChunkGroup& chunks, const Timer& timer, InstancedModel& model) noexcept
{
    std::vector<Vertex> tempData;

    for (size_t i = 0; i < particles.size(); ++i)
    {
        if (particles[i].isDead())
        {
            particles.erase(particles.begin() + i);
            continue;
        }

        particles[i].update(chunks, timer);
        
        tempData.emplace_back(glm::vec3{particles[i].position.x, particles[i].position.y, 0.0f},
                              glm::vec2{0.0f, 0.0f});
    }
    
    model.setInstanceData(tempData);
}

void ParticleGroup::draw(const Graphics& graphics, const InstancedModel& model) const noexcept
{
    if (particles.size() > 0)
    {
        model.drawInstanced(graphics.uniforms.model, graphics.uniforms.tex);
    }
}
