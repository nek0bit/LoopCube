#pragma once
#include <glm/glm.hpp>

struct Transform
{
    Transform(const glm::vec3& translate = {0.0f, 0.0f, 0.0f},
              const glm::vec3& scale = {0.0f, 0.0f, 0.0f},
              const glm::vec2& texturePos = {0.0f, 0.0f})
        : translate{translate},
          scale{scale},
          texturePos{texturePos}
        {}
    
    const glm::vec3 translate;
    const glm::vec3 scale;
    const glm::vec2 texturePos;
};
