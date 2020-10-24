#ifndef SPRITESHEET_HPP
#include <iostream>

// Note: This uses single-precision, I personally didn't see the reason to use double precision numbers but if there are errors in the future consult here
class Spritesheet {
public:
	Spritesheet(float width, float height);
	~Spritesheet();

	void set_x(int x);
    void set_y(int y);

	// Getters
    float get_x();
    float get_y();
	float get_width();
	float get_height();
private:
	float x;
    float y;
    float width;
    float height;
};

#endif
