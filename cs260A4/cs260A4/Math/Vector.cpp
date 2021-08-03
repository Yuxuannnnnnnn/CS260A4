#include "Vector.h"
#include "Math.h"

/*
Vector2::Vector2() : x{ 0 }, y{ 0 }
{
}

Vector2::Vector2(float v0) : x{ v0 }, y{ v0 }
{
}

Vector2::Vector2(float v0, float v1) : x{ v0 }, y{ v1 }
{
}

Vector2::Vector2(Vector3 v) : x{ v.x }, y{ v.y }
{
}

const Vector2 Vector2::Zero()
{
	return { 0, 0 };
}

const Vector2 Vector2::One()
{
	return { 1, 1 };
}

const Vector2 Vector2::Up()
{
	return { 0, 1 };
}

const Vector2 Vector2::Down()
{
	return { 0, -1 };
}

const Vector2 Vector2::Left()
{
	return { -1, 0 };
}

const Vector2 Vector2::Right()
{
	return { 1, 0 };
}

Vector2 Vector2::Normalize()
{
	if (SqrMagnitude() < Math::epsilon)
		return *this;
	return *this /= Magnitude();
}

float Vector2::Length() const
{
	return x * x + y * y;
}

Vector2 Vector2::normalized() const
{
	if (SqrMagnitude() < Math::epsilon)
		return *this;
	Vector2 v{ x, y };
	return v /= Magnitude();
}

float Vector2::SqrMagnitude() const
{
	return x * x + y * y;
}

float Vector2::Magnitude() const
{
	return sqrtf(SqrMagnitude());
}

float Vector2::DotProduct(const Vector2& a) const
{
	return x + a.x + y + a.y;
}

Vector2 Vector2::CrossProduct_Vector(const Vector2& a) const
{
	return Vector2{ a.y, -a.x };
}

float Vector2::CrossProduct_Magnitude(const Vector2& a) const
{
	return (x * a.y) - (y * a.x);
}

Vector2 Vector2::Square()
{
	x *= x;
	y *= y;

	return *this;
}

float Vector2::Distance(Vector2 a, Vector2 b)
{
	return (b - a).Magnitude();
}

Vector2 Vector2::Lerp(Vector2 a, Vector2 b, float t)
{
	return a + (b - a) * t;
}

#pragma region Vector2 Operators
Vector2& Vector2::operator=(const Vector2& a)
{
	x = a.x;
	y = a.y;
	return *this;
}

Vector2 Vector2::operator+(const Vector2 a) const
{
	Vector2 v;
	v.x = x + a.x;
	v.y = y + a.y;
	return v;
}

Vector2 Vector2::operator+(const float f)
{
	x += f;
	y += f;
	return *this;
}

Vector2 Vector2::operator+=(const Vector2 a)
{
	x += a.x;
	y += a.y;
	return *this;
}

Vector2 Vector2::operator-(const Vector2 a) const
{
	Vector2 v;
	v.x = x - a.x;
	v.y = y - a.y;
	return v;
}

Vector2 Vector2::operator-=(const Vector2 a)
{
	x -= a.x;
	y -= a.y;
	return *this;
}

Vector2 Vector2::operator-() const
{
	Vector2 v;
	v.x = -x;
	v.y = -y;
	return v;
}

Vector2 Vector2::operator-(const float f)
{
	x -= f;
	y -= f;
	return *this;
}

Vector2 Vector2::operator*(const Vector2 a) const
{
	Vector2 v;
	v.x = x * a.x;
	v.y = y * a.y;
	return v;
}

Vector2 Vector2::operator*(const float f) const
{
	Vector2 v;
	v.x = x * f;
	v.y = y * f;
	return v;
}

Vector2 Vector2::operator*=(const Vector2 a)
{
	x *= a.x;
	y *= a.y;
	return *this;
}

Vector2 Vector2::operator*=(const float f)
{
	x *= f;
	y *= f;
	return *this;
}

Vector2 Vector2::operator/(const float f) const
{
	Vector2 v;
	v.x = x / f;
	v.y = y / f;
	return v;
}

Vector2 Vector2::operator/=(const float f)
{
	x /= f;
	y /= f;
	return *this;
}

bool Vector2::operator!=(const Vector2 v) const
{
	if (x == v.x && y == v.y)
		return false;

	return true;
}

bool Vector2::operator==(const Vector2 v) const
{
	if (x == v.x && y == v.y)
		return true;

	return false;
}

bool Vector2::operator>(const Vector2 v) const
{
	if (this->x > v.x && this->y > v.y)
	{
		return true;
	}
	return false;
}

bool Vector2::operator>(const float f) const
{
	if (this->x > f && this->y > f)
	{
		return true;
	}
	return false;
}*/
#pragma endregion

Vector3::Vector3() : x{ 0 }, y{ 0 }, z{ 0 }
{
}

Vector3::Vector3(float v0) : x{ v0 }, y{ v0 }, z{ v0 }
{
}

Vector3::Vector3(float v0, float v1) : x{ v0 }, y{ v1 }, z{ 0 }
{
}

Vector3::Vector3(float v0, float v1, float v2) : x{ v0 }, y{ v1 }, z{ v2 }
{
}

Vector3::Vector3(Vector2 v) : x{ v.x }, y{ v.y }, z{ 0 }
{
}

const Vector3 Vector3::Zero()
{
	return { 0, 0 };
}

const Vector3 Vector3::One()
{
	return { 1, 1, 1 };
}

const Vector3 Vector3::Up()
{
	return { 0, 1 };
}

const Vector3 Vector3::Down()
{
	return { 0, -1 };
}

const Vector3 Vector3::Left()
{
	return { -1, 0 };
}

const Vector3 Vector3::Right()
{
	return { 1, 0 };
}

const Vector3 Vector3::Forward()
{
	return { 0, 0, 1 };
}

const Vector3 Vector3::Back()
{
	return { 0, 0, -1 };
}

Vector3 Vector3::Normalize()
{
	if (SqrMagnitude() < Math::epsilon)
		return *this;
	return *this /= Magnitude();
}

Vector3 Vector3::normalized() const
{
	if (SqrMagnitude() < Math::epsilon)
		return *this;
	Vector3 v{ x, y, z };
	return v /= Magnitude();
}

float Vector3::SqrMagnitude() const
{
	return x * x + y * y + z * z;
}

float Vector3::Magnitude() const
{
	return sqrtf(SqrMagnitude());
}

float Vector3::Dot(Vector3 a, Vector3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Vector3::MoveTowards(Vector3 a, Vector3 b, float t)
{
	return a + (b - a).normalized() * t;
}

Vector3 Vector3::Lerp(Vector3 a, Vector3 b, float t)
{
	return a + (b - a) * t;
}

#pragma region Vector3 Operators
Vector3& Vector3::operator=(const Vector3& a)
{
	x = a.x;
	y = a.y;
	z = a.z;
	return *this;
}

Vector3& Vector3::operator=(const Vector2& a)
{
	x = a.x;
	y = a.y;
	z = 0;
	return *this;
}

Vector3 Vector3::operator+(const Vector3 a) const
{
	Vector3 v;
	v.x = x + a.x;
	v.y = y + a.y;
	v.z = z + a.z;
	return v;
}

Vector3 Vector3::operator+=(const Vector3 a)
{
	x += a.x;
	y += a.y;
	z += a.z;
	return *this;
}

Vector3 Vector3::operator-(const Vector3 a) const
{
	Vector3 v;
	v.x = x - a.x;
	v.y = y - a.y;
	v.z = z - a.z;
	return v;
}

Vector3 Vector3::operator-=(const Vector3 a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	return *this;
}

Vector3 Vector3::operator-() const
{
	Vector3 v;
	v.x = -x;
	v.y = -y;
	v.z = -z;
	return v;
}

Vector3 Vector3::operator*(const Vector3 a) const
{
	Vector3 v;
	v.x = x * a.x;
	v.y = y * a.y;
	v.z = z * a.z;
	return v;
}

Vector3 Vector3::operator*(const float f) const
{
	Vector3 v;
	v.x = x * f;
	v.y = y * f;
	v.z = z * f;
	return v;
}

Vector3 Vector3::operator*=(const Vector3 a)
{
	x *= a.x;
	y *= a.y;
	z *= a.z;
	return *this;
}

Vector3 Vector3::operator*=(const float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

Vector3 Vector3::operator/(const float f) const
{
	Vector3 v;
	v.x = x / f;
	v.y = y / f;
	v.z = z / f;
	return v;
}

Vector3 Vector3::operator/=(const float f)
{
	x /= f;
	y /= f;
	z /= f;
	return *this;
}
#pragma endregion

Vector4::Vector4() : x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 }
{
}

Vector4::Vector4(float v0) : x{ v0 }, y{ v0 }, z{ v0 }, w{ v0 }
{
}

Vector4::Vector4(float v0, float v1, float v2) : x{ v0 }, y{ v1 }, z{ v2 }, w{ 0 }
{
}

Vector4::Vector4(float v0, float v1, float v2, float v3) : x{ v0 }, y{ v1 }, z{ v2 }, w{ v3 }
{
}