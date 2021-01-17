#include "camera.hpp"

Camera::Camera(WinSize* winSize)
    : Vec2{0, 0},
      winSize{winSize}
{}

int Camera::getWidth()
{
    return winSize->w;
}

int Camera::getHeight()
{
    return winSize->h;
}
