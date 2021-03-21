#include "model.hpp"

Model::Model(const GLuint shader, const std::vector<Vertex>& vertices)
    : refCount{std::make_shared<int>(1)},
      vao{0},
      vbo{0},
      size{0},
      shader{shader}
{
    if (shader != 0)
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        
        if (vertices.size() > 0)
            setBufferData(vertices);
    }
}

Model::~Model()
{
    if (*refCount == 1 && shader != 0)
    {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    (*refCount)--;
}

// Copy
Model::Model(const Model& source)
    : refCount{source.refCount},
      vao{source.vao},
      vbo{source.vbo},
      size{source.size},
      shader{source.shader}
{
    (*refCount)++;
}

// Move
Model::Model(Model&& source)
    : refCount{std::move(source.refCount)},
      vao{std::move(source.vao)},
      vbo{std::move(source.vbo)},
      size{std::move(source.size)},
      shader{std::move(source.shader)}
{
    // Probably simpler to do it this way
    (*refCount)++;
}

void Model::setBufferData(const std::vector<Vertex>& vertices, const GLenum usage)
{
    if (shader != 0)
    {
        // Bind both values
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
        size = vertices.size(); // Used for glDrawArrays size
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], usage);
        setupVertexLayout();
    }
}

void Model::setupVertexLayout()
{
    constexpr uint8_t Stride = sizeof(Vertex);
    constexpr uint8_t positionSize = 3;
    constexpr uint8_t texCoordOffset = sizeof(glm::vec3); // First element in struct
    constexpr uint8_t texCoordSize = 2;
    const GLuint positionAttribute = glGetAttribLocation(shader, "position");
    const GLuint texCoordAttribute = glGetAttribLocation(shader, "texCoord");
    
    // Position
    // InputAttrib - valSize - Type - shouldNormalize - Stride - Offset
    glVertexAttribPointer(positionAttribute, positionSize, GL_FLOAT, GL_FALSE, Stride, 0);
    
    // texCoord
    glVertexAttribPointer(texCoordAttribute, texCoordSize, GL_FLOAT, GL_FALSE,
                          Stride, (void*)(texCoordOffset));

    // Enable Attrib Arrays
    glEnableVertexAttribArray(positionAttribute);
    glEnableVertexAttribArray(texCoordAttribute);
}

void Model::draw(const GLint& uModel,
                 const GLint& uTex,
                 const glm::vec3& translate,
                 const glm::vec3& scale,
                 const glm::vec2& texturePos) const noexcept
{
    glBindVertexArray(vao);
    glm::mat4 model{1.0f};

    model = glm::translate(model, translate);
    model = glm::scale(model, scale);    
    
    glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniform2fv(uTex, 1, glm::value_ptr(texturePos));

    glDrawArrays(GL_TRIANGLES, 0, size);
}
