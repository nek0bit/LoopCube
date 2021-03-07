#pragma once
#include <iostream>
#include <vector>
#include <cstdint>

#include "../include/glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL2/SDL_opengl.h>

#include "vertex.hpp"

struct Model
{
    Model(const GLuint shader, const std::vector<Vertex>& vertices = {});
    Model(const Model&) = delete;
    Model(Model&& source);
    ~Model();

    void setupVertexLayout();
    void setBufferData(const std::vector<Vertex>& vertices, const GLenum usage = GL_STATIC_DRAW);
    void draw(const GLint& uModel,
              const GLint& uTex,
              const glm::vec3& translate = {1.0f, 1.0f, 1.0f},
              const glm::vec3& scale = {1.0f, 1.0f, 1.0f},
              const glm::vec2& texturePos = {1.0f, 1.0f}) const noexcept;

    GLuint vao, vbo;
    uint16_t size;
    const GLuint shader;
};
