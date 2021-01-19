#include "transition.hpp"

Transition::Transition(double value, double threshold)
    : value{value},
      threshold{threshold},
      at{value}
{}

Transition::~Transition() {}

void Transition::update(Timer& timer)
{
	at += ((value - at) * threshold) * timer.deltaTime.s;
}

double Transition::get()
{
	return at;
}
