#include "state.hpp"

State::State() : state{0} {

}

State::~State() {}

void State::set(int new_state) {
	state = new_state;
}

int State::get() const {
	return state;
}

// Operators
bool operator==(const State &state, const int &compare) {
	return state.get() == compare;
}

bool operator!=(const State &state, const int &compare) {
	return !(state == compare);
}
