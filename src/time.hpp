#pragma once

#include "timer.hpp"

enum time_state {
	TIME_DAY, TIME_NIGHT
};

struct Time {
	Time(float startTime, float maxTime, float morningTime,
         float nightTime, float morningOffset, float nightOffset);
	~Time();

	void tick(Timer& timer, int amount = 1);

    time_state state;
    float time;

	const float maxTime;
	const float morningTime;
	const float nightTime;
	const float morningOffset;
	const float nightOffset;
};
