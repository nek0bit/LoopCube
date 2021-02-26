#include "camera.hpp"

Camera::Camera(WinSize& winSize) noexcept
    : size{winSize},
      // Vec
      position{0.0f, 0.0f, 0.0f},
      center{0.0f, 0.0f, 0.0f},
      up{0.0f, 0.0f, 1.0f},
      // Mat
      projection{1.0f},
      view{1.0f}
{
    updateProj();
}

Camera::~Camera() {}

void Camera::updateProj() noexcept
{
    projection = glm::perspective(glm::radians(45.0f),
                                  static_cast<float>(size.w) / static_cast<float>(size.h),
                                  1.0f, 10.0f);
}

void Camera::bindCamera(Graphics& graphics) noexcept
{
    GLint uView = graphics.getUniformLocation("view");
    glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(view));
}
