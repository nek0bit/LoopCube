#ifndef STATE_HPP
#define STATE_HPP

class State {
public:
	State();
	~State();

	void set(int new_state);
	int get() const;
	
	friend bool operator==(const State &state, const int &compare);
	friend bool operator!=(const State &state, const int &compare);
private:
	int state;

};

#endif // STATE_HPP
