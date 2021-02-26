#pragma once

#include "timer.hpp"

enum time_state {
	TIME_DAY, TIME_NIGHT
};

struct Time {
	Time(const float startTime, const float maxTime, const float morningTime,
         const float nightTime, const float morningOffset, const float nightOffset) noexcept;
	~Time();

	void tick(const Timer& timer, int amount = 1) noexcept;

    time_state state;
    float time;

	const float maxTime;
	const float morningTime;
	const float nightTime;
	const float morningOffset;
	const float nightOffset;
};
