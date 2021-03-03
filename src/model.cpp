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

void Model::draw(const GLint& uniform, const glm::vec3& translate, const glm::vec3& scale) const noexcept
{
    glm::mat4 model{1.0f};

    model = glm::translate(model, translate);
    model = glm::scale(model, scale);
    
    
    glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(model));
    
    glDrawArrays(GL_TRIANGLES, 0, size);
}
