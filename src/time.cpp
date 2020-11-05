#include "time.hpp"

Time::Time(int start_time, int max_time,
		   int morning_time, int night_time,
		   int morning_offset, int night_offset) : time{start_time}, max_time{max_time},
												   morning_time{morning_time}, night_time{night_time},
												   morning_offset{morning_offset}, night_offset{night_offset} {}

Time::~Time() {}

void Time::tick(int amount) {
	time += amount;
	if (time > max_time) time = 0;

	if (time > morning_time && time < night_time) {
		state = TIME_DAY;
	} else if (time > night_time || time < morning_time) {
		state = TIME_NIGHT;
	}
}
