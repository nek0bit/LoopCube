#include "time.hpp"

Time::Time(float startTime, float maxTime,
		   float morningTime, float nightTime,
		   float morningOffset, float nightOffset)
    : time{startTime},
      maxTime{maxTime},
      morningTime{morningTime},
      nightTime{nightTime},
      morningOffset{morningOffset},
      nightOffset{nightOffset}
{}

Time::~Time() {}

void Time::tick(Timer& timer, int amount) {
	time += amount * timer.deltaTime.s;
	if (time > maxTime) time = 0;

	if (time > morningTime && time < nightTime) {
		state = TIME_DAY;
	} else if (time > nightTime || time < morningTime) {
		state = TIME_NIGHT;
	}
}
