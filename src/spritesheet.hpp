#pragma once
#include <iostream>

struct Spritesheet
{
	Spritesheet(float width, float height);
	~Spritesheet();

    float getX();
    float getY();
    
	float x;
    float y;
    float width;
    float height;
};
