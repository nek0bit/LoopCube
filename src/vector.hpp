#pragma once

struct Vec2
{
    Vec2();
    Vec2(double x, double y);
    ~Vec2() = default;

    // Vector operations overloads
    Vec2 operator+(const Vec2& add) const;
    Vec2 operator-(const Vec2& sub) const;
    Vec2 operator*(const Vec2& mul) const;
    Vec2 operator/(const Vec2& div) const;

    // Scalar operations overloads
    Vec2 operator+(const double& add) const;
    Vec2 operator-(const double& sub) const;
    Vec2 operator*(const double& mul) const;
    Vec2 operator/(const double& div) const;
    
	double x, y;
};

