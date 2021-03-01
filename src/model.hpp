#pragma once

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
    void render() const noexcept;
    void bindDraw() const noexcept;
private:
    uint16_t size;
    GLuint vbo;
};
