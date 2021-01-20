#pragma once

#include "timer.hpp"

struct Animation
{
	Animation() = default;
	Animation(int maximum);

	int tick(Timer& timer, float amount = 1);
    
    float frame;
	int maxFrames;
};
