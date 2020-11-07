#ifndef TRANSITION_HPP
#define TRANSITION_HPP

class Transition {
public:
	Transition() = default;
	Transition(double value, double threshold);
	~Transition();

	void update();
	double get();
	
	double value;
	double threshold;
private:
    double at;
};

#endif
