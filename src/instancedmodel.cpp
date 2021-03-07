#include "instancedmodel.hpp"

InstancedModel::InstancedModel(const GLuint shader, const std::vector<Vertex>& vertices)
    : Model{shader, vertices},
      positionBuffer{0},
      textureBuffer{0}
{
    glGenBuffers(1, &positionBuffer);

}

InstancedModel::~InstancedModel()
{
    glDeleteBuffers(1, &positionBuffer);
    glDeleteBuffers(1, &textureBuffer);
}

void InstancedModel::setInstanceData(const std::vector<Vertex>& instances)
{
    constexpr uint8_t Stride = sizeof(Vertex);
    constexpr uint8_t positionSize = 3;
    constexpr uint8_t texCoordOffset = sizeof(glm::vec3); // First element in struct
    constexpr uint8_t texCoordSize = 2;    
    const GLuint positionAttribute = glGetAttribLocation(shader, "position");
    const GLuint texCoordAttribute = glGetAttribLocation(shader, "texCoord");

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * instances.size(), &instances[0], GL_STREAM_DRAW);

    glEnableVertexAttribArray(positionAttribute);
    glEnableVertexAttribArray(texCoordAttribute);

    glVertexAttribPointer(positionAttribute, positionSize, GL_FLOAT, GL_FALSE, Stride, 0);
    glVertexAttribPointer(texCoordAttribute, texCoordSize, GL_FLOAT, GL_FALSE, Stride, (void*)(texCoordOffset));

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
}

void InstancedModel::draw(const int amount)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, amount);
}
