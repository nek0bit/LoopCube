#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "graphics.hpp"
#include "winsize.hpp"

struct Camera
{
	Camera() = default;
	Camera(WinSize& winSize) noexcept;
    ~Camera();
    
    void updateProj() noexcept;
    void bindCamera(Graphics& graphics) noexcept;
    
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
