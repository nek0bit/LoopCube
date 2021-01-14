#include "spritesheet.hpp"

Spritesheet::Spritesheet(float width, float height) : x{0}, y{0}, width{width}, height{height} {}
Spritesheet::~Spritesheet(){}

float Spritesheet::getX()
{
	return x * width;
}


float Spritesheet::getY()
{
	return y * height;
}
