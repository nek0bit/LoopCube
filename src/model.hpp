#pragma once
#include <iostream>
#include <vector>
#include <cstdint>

#include "../include/glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vertex.hpp"

struct Model
{
    Model(const std::vector<Vertex>& vertices);
    Model(const Model&) = delete;
    Model(Model&& source);
    ~Model();

    void setModel(const std::vector<Vertex>& vertices);
    void draw(const GLint& uModel,
              const GLint& uTex,
              const glm::vec3& translate = {0.0f, 0.0f, 0.0f},
              const glm::vec3& scale = {0.0f, 0.0f, 0.0f},
              const glm::vec2& texturePos = {1.0f, 1.0f}) const noexcept;

    GLuint vbo;
    uint16_t size;
};
