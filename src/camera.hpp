#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <iostream>
#include <SDL2/SDL.h>

class Camera {

public:
	Camera() {};
	Camera(int* width, int* height);
	~Camera();

	void set_pos(double x, double y);
	double get_x();
	double get_y();

	int get_width();
	int get_height();

private:

	double x_pos;
	double y_pos;

	int* width;
	int* height;
};


#endif // CAMERA_HPP
