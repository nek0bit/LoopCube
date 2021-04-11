#include "instancedmodel.hpp"

InstancedModel::InstancedModel(const GLuint shader, const std::vector<Vertex>& vertices)
    : Model{shader, vertices},
      positionBuffer{0},
      textureBuffer{0},
      instanceSize{0}
{
    glBindVertexArray(vao);
    glGenBuffers(1, &positionBuffer);
}

InstancedModel::~InstancedModel()
{
    glDeleteBuffers(1, &positionBuffer);
}

void InstancedModel::setInstanceData(const std::vector<Vertex>& instances)
{
    constexpr uint8_t Stride = sizeof(Vertex);
    constexpr uint8_t positionSize = 3;
    constexpr uint8_t texCoordOffset = sizeof(glm::vec3); // First element in struct
    constexpr uint8_t texCoordSize = 2;    
    const GLuint positionAttribute = glGetAttribLocation(shader, "instanceTrans");

    glBindVertexArray(vao);
    instanceSize = instances.size();
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * instances.size(), &instances[0], GL_STREAM_DRAW);

    glVertexAttribPointer(positionAttribute, positionSize, GL_FLOAT, GL_FALSE, Stride, 0);
    
    glEnableVertexAttribArray(positionAttribute);

    glVertexAttribDivisor(positionAttribute, 1);
}

void InstancedModel::drawInstanced(const GLint& uModel,
                                   const GLint& uTex) const
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glm::mat4 model{1.0f};
    glm::vec2 tex{1.0f, 1.0f};
    
    glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniform2fv(uTex, 1, glm::value_ptr(tex));
    
    glDrawArraysInstanced(GL_TRIANGLES, 0, size, instanceSize);
}
