#pragma once

#include "winsize.hpp"
#include "vector.hpp"

struct Camera: public Vec2
{
	Camera() = default;
	Camera(WinSize* winSize);
    
    int getWidth();
	int getHeight();
private:
    WinSize* winSize;
};
