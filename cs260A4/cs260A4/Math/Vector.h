#pragma once
#include "../physics/Vector2.h"

struct Vector3;
/*
struct Vector2
{
	float x, y;
	Vector2();
	Vector2(float);
	Vector2(float, float);
	Vector2(Vector3);
	static const Vector2 Zero();
	static const Vector2 One();
	static const Vector2 Up();
	static const Vector2 Down();
	static const Vector2 Left();
	static const Vector2 Right();
	Vector2 Normalize();
	float Length() const;
	Vector2 normalized() const;
	float Magnitude() const;
	float SqrMagnitude() const;
	float DotProduct(const Vector2& a) const;
	float CrossProduct_Magnitude(const Vector2& a) const;
	Vector2 CrossProduct_Vector(const Vector2& a) const;
	Vector2 Square();
	static float Distance(Vector2 a, Vector2 b);
	static Vector2 Lerp(Vector2 a, Vector2 b, float t);
#pragma region Vector2 Operators
	Vector2& operator=(const Vector2& a);
	Vector2 operator+(Vector2 a) const;
	Vector2 operator+(float f);
	Vector2 operator+=(Vector2 a);
	Vector2 operator-(Vector2 a) const;
	Vector2 operator-=(Vector2 a);
	Vector2 operator-() const;
	Vector2 operator-(float f);
	Vector2 operator*(Vector2 f) const;
	Vector2 operator*(float f) const;
	Vector2 operator*=(Vector2 f);
	Vector2 operator*=(float f);
	Vector2 operator/(float f) const;
	Vector2 operator/=(float f);
	bool operator>(Vector2 v) const;
	bool operator>(float f) const;
	bool operator==(Vector2 v) const;
	bool operator!=(Vector2 v) const;
#pragma endregion
};
*/
struct Vector3
{
	float x, y, z;
	Vector3();
	Vector3(float);
	Vector3(float, float);
	Vector3(float, float, float);
	Vector3(Vector2);
	static const Vector3 Zero();
	static const Vector3 One();
	static const Vector3 Up();
	static const Vector3 Down();
	static const Vector3 Left();
	static const Vector3 Right();
	static const Vector3 Forward();
	static const Vector3 Back();
	Vector3 Normalize();
	Vector3 normalized() const;
	float Magnitude() const;
	float SqrMagnitude() const;
	static float Dot(Vector3 a, Vector3 b);
	static Vector3 MoveTowards(Vector3 from, Vector3 to, float speed);
	static Vector3 Lerp(Vector3 a, Vector3 b, float t);
#pragma region Vector3 Operators
	Vector3& operator=(const Vector3& a);
	Vector3& operator=(const Vector2& a);
	Vector3 operator+(Vector3 a) const;
	Vector3 operator+=(Vector3 a);
	Vector3 operator-(Vector3 a) const;
	Vector3 operator-=(Vector3 a);
	Vector3 operator-() const;
	Vector3 operator*(Vector3 f) const;
	Vector3 operator*(float f) const;
	Vector3 operator*=(Vector3 f);
	Vector3 operator*=(float f);
	Vector3 operator/(float f) const;
	Vector3 operator/=(float f);
#pragma endregion
};

struct Vector4
{
	float x, y, z, w;
	Vector4();
	Vector4(float);
	Vector4(float, float, float);
	Vector4(float, float, float, float);
};
