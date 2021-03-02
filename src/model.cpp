#include "model.hpp"

Model::Model(const std::vector<Vertex>& vertices)
{
    glGenBuffers(1, &vbo);
    setModel(vertices);
}

Model::~Model()
{
    glDeleteBuffers(1, &vbo);
}

void Model::setModel(const std::vector<Vertex>& vertices)
{
    size = vertices.size();
    bind();
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
}

void Model::bind() const noexcept
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void Model::render() const noexcept
{
    glDrawArrays(GL_TRIANGLES, 0, size);
}

void Model::bindDraw() const noexcept
{
    bind();
    render();
}
