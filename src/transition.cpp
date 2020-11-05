#include "transition.hpp"

Transition::Transition(double value, double threshold) : value{value}, threshold{threshold}, at{value} {}

Transition::~Transition() {}

void Transition::update() {
	at += (value - at) * threshold;
}

double Transition::get() {
	return at;
}
