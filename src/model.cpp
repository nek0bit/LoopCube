#include "model.hpp"

Model::Model(const std::vector<Vertex>& vertices)
    : vbo{0},
      size{0}
{
    glGenBuffers(1, &vbo);
    setModel(vertices);
}

Model::~Model()
{
    glDeleteBuffers(1, &vbo);
}

// Move
Model::Model(Model&& source)
{
    vbo = source.vbo;
    size = source.size;

    // Sets source to 0 (glDeleteBuffers ignores 0)
    source.vbo = 0;
    source.size = 0;
}

void Model::setModel(const std::vector<Vertex>& vertices)
{
    size = vertices.size();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
}

void Model::draw(const GLint& uModel,
                 const GLint& uTex,
                 const glm::vec3& translate,
                 const glm::vec3& scale,
                 const glm::vec2& texturePos) const noexcept
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glm::mat4 model{1.0f};

    model = glm::translate(model, translate);
    model = glm::scale(model, scale);    
    
    glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniform2fv(uTex, 1, glm::value_ptr(texturePos));
    
    glDrawArrays(GL_TRIANGLES, 0, size);
}
