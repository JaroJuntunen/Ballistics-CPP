#pragma once

struct Vec2 {
	float x, y;

	Vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

Vec2 operator+(const Vec2& other) const { return {x + other.x, y + other.y}; }
	Vec2 operator-(const Vec2& other) const { return {x - other.x, y - other.y}; }
	Vec2& operator+=(const Vec2& other) { x += other.x; y += other.y; return *this; }
	Vec2& operator-=(const Vec2& other) { x -= other.x; y -= other.y; return *this; }
};
