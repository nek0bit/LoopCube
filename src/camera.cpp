#include "camera.hpp"

Camera::Camera(int* width, int* height) {
	this->width = width;
	this->height = height;
}

Camera::~Camera() {
}

void Camera::set_pos(double x, double y) {
	x_pos = x;
	y_pos = y;
}

double Camera::get_x() {
	return x_pos;
}

double Camera::get_y() {
	return y_pos;
}

int Camera::get_width() {
	return *width;
}

int Camera::get_height() {
	return *height;
}
