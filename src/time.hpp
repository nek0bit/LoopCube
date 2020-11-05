#ifndef _TIME_HPP
#define _TIME_HPP

enum time_state {
	TIME_DAY, TIME_NIGHT
};

struct Time {
	Time(int start_time, int max_time, int morning_time, int night_time, int morning_offset, int night_offset);
	~Time();

	void tick(int amount = 1);

	int state;
	int time;

	const int max_time;
	const int morning_time;
	const int night_time;
	const int morning_offset;
	const int night_offset;
};

#endif
