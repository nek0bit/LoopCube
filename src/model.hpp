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
    ~Model();

    void setModel(const std::vector<Vertex>& vertices);
    void bind() const noexcept;
    void draw(const GLint& uniform,
              const glm::vec3& translate = {0.0f, 0.0f, 0.0f},
              const glm::vec3& scale = {0.0f, 0.0f, 0.0f}) const noexcept;
private:
    uint16_t size;
    GLuint vbo;
};
