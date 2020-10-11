#include "collisioninfo.hpp"

CollisionInfo::CollisionInfo() : colliding{false}, top{-1}, bottom{-1}, left{-1}, right{-1} {}

CollisionInfo::CollisionInfo(double top, double bottom, double left, double right) : colliding{true}, top{top}, bottom{bottom},
																					 left{left}, right{right} {}

CollisionInfo::~CollisionInfo() {

}

bool CollisionInfo::operator==(const bool& rhs) const {
	return colliding;
}
