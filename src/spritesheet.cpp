#include "spritesheet.hpp"

Spritesheet::Spritesheet(float width, float height) : x{0}, y{0}, width{width}, height{height} {}
Spritesheet::~Spritesheet(){}

void Spritesheet::set_x(int x) {
	this->x = x;
}

void Spritesheet::set_y(int y) {
	this->y = y;
}

float Spritesheet::get_x() {
	return x*width;
}


float Spritesheet::get_y() {
	return y*height;
}

float Spritesheet::get_width() {
	return width;
}


float Spritesheet::get_height() {
	return height;
}
