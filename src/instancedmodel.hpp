#pragma once
#include "model.hpp"

struct InstancedModel: public Model
{
    InstancedModel(const GLuint shader, const std::vector<Vertex>& vertices = {});
    ~InstancedModel();

    void setInstanceData(const std::vector<Vertex>& instances = {});
    void drawInstanced(const GLint& uModel,
                       const GLint& uTex) const;

    GLuint positionBuffer;
    GLuint textureBuffer;
    GLuint instanceSize;
};
