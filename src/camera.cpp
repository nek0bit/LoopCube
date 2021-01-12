#include "camera.hpp"

Camera::Camera(WinSize* winSize)
    : winSize{winSize}
{}

int Camera::getWidth()
{
    return winSize->w;
}

int Camera::getHeight()
{
    return winSize->h;
}
