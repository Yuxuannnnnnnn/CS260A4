#pragma once

struct Vector2
{
	Vector2();
	Vector2(float x_, float y_);

	Vector2 operator+(const Vector2& rhs);
	Vector2 operator-(const Vector2& rhs);
	Vector2 operator*(float scalar);

	float Dot(const Vector2& rhs);

	float GetMagnitude() const;
	void Normalize();

	float x;
	float y;
};