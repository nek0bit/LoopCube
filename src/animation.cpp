#include "animation.hpp"

Animation::Animation(int maximum)
    : frame{0},
      maxFrames{maximum}
{}

int Animation::tick(int amount)
{
	frame += amount;

	if (frame > maxFrames)
    {
		frame = 0;
	}

	return frame;
}
