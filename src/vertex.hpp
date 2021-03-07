#pragma once
#include <glm/glm.hpp>

struct Vertex
{
    Vertex(const glm::vec3& position, const glm::vec2& coords)
        : position{position}, coords{coords} {}
    
    glm::vec3 position;
    glm::vec2 coords;
};
