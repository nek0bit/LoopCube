#include "time.hpp"

Time::Time(int start_time, int max_time,
		   int morning_time, int night_time) : time{start_time}, max_time{max_time},
											   morning_time{morning_time}, night_time{night_time} {}

Time::~Time() {}

void Time::tick(int amount) {
	time += amount;
	if (time > max_time) time = 0;
}
