#include "camera.hpp"

Camera::Camera(WinSize& winSize)
    : size{winSize},
      // Vec
      position{0.0f, 0.0f, 0.0f},
      center{0.0f, 0.0f, 0.0f},
      up{0.0f, 1.0f, 0.0f},
      // Mat
      projection{1.0f},
      view{1.0f}
{
    updateProj();
}

Camera::~Camera() {}

void Camera::updateProj()
{
    if (size.w == 0 || size.h == 0) return;
    /*projection = glm::perspective(glm::radians(45.0f),
                                  static_cast<float>(size.w) / static_cast<float>(size.h),
                                  1.0f, 10.0f);*/
    projection = glm::ortho(0.0f, static_cast<float>(size.w),
                            static_cast<float>(size.h), 0.0f,
                            0.1f, 5000.0f);
}

void Camera::bindZoom(const GLint& uZoom, const glm::vec2& zoom) noexcept
{
    glUniform2fv(uZoom, 1, glm::value_ptr(zoom));
}

void Camera::bindProj(const GLint& uProj) noexcept
{
    glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
}

void Camera::bindView(const GLint& uView) noexcept
{
    glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(view));
}
