#pragma once
#include <cmath>

struct	Vec2 {
	float	x, y;

	Vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

	//	Arithmetic
	Vec2	operator+(const Vec2& other)	const	{ return {x + other.x, y + other.y}; }
	Vec2	operator-(const Vec2& other)	const	{ return {x - other.x, y - other.y}; }
	Vec2&	operator+=(const Vec2& other)		{ x += other.x; y += other.y; return *this; }
	Vec2&	operator-=(const Vec2& other)		{ x -= other.x; y -= other.y; return *this; }
	Vec2	operator*(float s)		const	{ return {x * s, y * s}; }

	//	Utilities
	Vec2	normalized() const
	{
		float	len = std::sqrt(x * x + y * y);
		if (len == 0.0f) return {0.0f, 0.0f};
		return {x / len, y / len};
	}
};
