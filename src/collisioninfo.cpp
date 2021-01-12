#include "collisioninfo.hpp"

CollisionInfo::CollisionInfo()
    : top{-1},
      bottom{-1},
      left{-1},
      right{-1},
      colliding{false}
{}

CollisionInfo::CollisionInfo(double top, double bottom, double left, double right)
    : top{top},
      bottom{bottom},
      left{left},
      right{right},
      colliding{true}
{}

bool CollisionInfo::operator==(const bool& rhs) const
{
	return colliding == rhs;
}
