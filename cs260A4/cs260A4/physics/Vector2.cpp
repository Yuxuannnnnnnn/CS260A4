#include "Vector2.h"

#include <cmath>

Vector2::Vector2() :
	x{ 0.0f },
	y{ 0.0f }
{
}

Vector2::Vector2(float x_, float y_) :
	x{ x_ },
	y{ y_ }
{
}

Vector2 Vector2::operator+(const Vector2& rhs)
{
	return Vector2(x + rhs.x, y + rhs.y);
}

Vector2 Vector2::operator-(const Vector2& rhs)
{
	return Vector2(x - rhs.x, y - rhs.y);
}

Vector2 Vector2::operator*(float scalar)
{
	return Vector2(x * scalar, y * scalar);
}

float Vector2::Dot(const Vector2& rhs)
{
	return 0.0f;
}

float Vector2::GetMagnitude() const
{
	return sqrt(x * x + y * y);
}

void Vector2::Normalize()
{
	if (GetMagnitude() == 0.0f)
		return;

	x = x / GetMagnitude();
	y = y / GetMagnitude();
}