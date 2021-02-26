#include "time.hpp"

Time::Time(const float startTime, const float maxTime,
		   const float morningTime, const float nightTime,
		   const float morningOffset, const float nightOffset) noexcept
    : time{startTime},
      maxTime{maxTime},
      morningTime{morningTime},
      nightTime{nightTime},
      morningOffset{morningOffset},
      nightOffset{nightOffset}
{}

Time::~Time() {}

void Time::tick(const Timer& timer, int amount) noexcept
{
	time += amount * timer.deltaTime;
	if (time > maxTime) time = 0;

	if (time > morningTime && time < nightTime)
    {
		state = TIME_DAY;
	}
    else if (time > nightTime || time < morningTime)
    {
		state = TIME_NIGHT;
	}
}
