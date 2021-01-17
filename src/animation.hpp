#pragma once

struct Animation
{
	Animation() = default;
	Animation(int maximum);

	int tick(int amount = 1);
    
	int frame;
	int maxFrames;
};
