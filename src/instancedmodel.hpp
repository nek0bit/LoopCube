#pragma once
#include "model.hpp"

struct InstancedModel: public Model
{
    InstancedModel(const GLuint shader, const std::vector<Vertex>& vertices = {});
    ~InstancedModel();

    void setInstanceData(const std::vector<Vertex>& instances = {});
    void draw(const int amount);

    GLuint positionBuffer;
    GLuint textureBuffer;
    size_t instanceSize;
};
