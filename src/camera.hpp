#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../include/glad/glad.h"

#include "winsize.hpp"

struct Camera
{
	Camera(WinSize& winSize);
    ~Camera();
    
    void updateProj();
    void bindProj(const GLint& uProj) noexcept;
    void bindView(const GLint& uView) noexcept;
    void bindZoom(const GLint& uZoom, const glm::vec2& zoom) noexcept;
    
    inline void updateView() noexcept
        {
            view = glm::lookAt(position, center, up);
        }

    WinSize& size;

    // Position
    glm::vec3 position;
    glm::vec3 center;
    glm::vec3 up;
    
    glm::mat4 projection;
    glm::mat4 view;
};
