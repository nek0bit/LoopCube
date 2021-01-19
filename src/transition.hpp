#pragma once

#include "timer.hpp"

struct Transition
{
	Transition() = default;
	Transition(double value, double threshold);
	~Transition();

	void update(Timer& timer);
	double get();
	
	double value;
	double threshold;
private:
    double at;
};
