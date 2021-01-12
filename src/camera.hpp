#pragma once

#include "winsize.hpp"

struct Camera {
	Camera() = default;
	Camera(WinSize* winSize);
    
	double x;
	double y;

    int getWidth();
	int getHeight();
private:
    WinSize* winSize;
};
