#pragma once

struct CollisionInfo
{
	CollisionInfo();
	CollisionInfo(double top, double bottom, double left, double right);
	~CollisionInfo() = default;
    
	bool operator==(const bool& rhs) const;
	
	double top;
	double bottom;
	double left;
	double right;
	bool colliding;
};

