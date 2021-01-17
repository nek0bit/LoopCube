#include "vector.hpp"

Vec2::Vec2()
    : x{0},
      y{0}
{}

Vec2::Vec2(double x, double y)
    : x{x},
      y{y}
{}

// Vector operations
Vec2 Vec2::operator+(const Vec2& add) const
{
    return Vec2{x + add.x, y + add.y};
}

Vec2 Vec2::operator-(const Vec2& sub) const
{
    return Vec2{x - sub.x, y - sub.y};
}

Vec2 Vec2::operator*(const Vec2& mul) const
{
    return Vec2{x * mul.x, y * mul.y};
}

Vec2 Vec2::operator/(const Vec2& div) const
{
    return Vec2{x / div.x, y / div.y};
}

// Scalar operations
Vec2 Vec2::operator+(const double& add) const
{
    return Vec2{x + add, y + add};
}

Vec2 Vec2::operator-(const double& sub) const
{
    return Vec2{x - sub, y - sub};
}

Vec2 Vec2::operator*(const double& mul) const
{
    return Vec2{x * mul, y * mul};
}

Vec2 Vec2::operator/(const double& div) const
{
    return Vec2{x / div, y / div};
}
