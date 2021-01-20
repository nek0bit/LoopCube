#include "animation.hpp"

Animation::Animation(int maximum)
    : frame{0},
      maxFrames{maximum}
{}

int Animation::tick(Timer& timer, float amount)
{
	frame += amount * timer.deltaTime.s;

	if (frame > maxFrames)
    {
		frame = 0;
	}

	return frame;
}
