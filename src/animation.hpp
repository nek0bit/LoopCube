#pragma once

struct Animation
{
	Animation() = default;
	Animation(int maximum);

	int tick();
    
	int frame;
	int maxFrames;
};
