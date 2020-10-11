#ifndef COLLISIONINFO_HPP
#define COLLISIONINFO_HPP
#include <iostream>

#include <vector>

struct CollisionInfo {
	CollisionInfo();
	CollisionInfo(double top, double bottom, double left, double right);
	~CollisionInfo();
	bool operator==(const bool& rhs) const;
	
	double top;
	double bottom;
	double left;
	double right;
	bool colliding;
};

#endif
