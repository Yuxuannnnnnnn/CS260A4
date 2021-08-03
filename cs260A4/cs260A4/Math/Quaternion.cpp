#include "Quaternion.h"
#include "Vector.h"
#include "Math.h"

Quaternion::Quaternion()
	: x{ 0 }, y{ 0 }, z{ 0 }, w{ 1 } {}

Quaternion::Quaternion(float q0, float q1, float rhs, float q3)
	: x{ q0 }, y{ q1 }, z{ rhs }, w{ q3 } {}

const Quaternion Quaternion::Identity()
{
	return { 0, 0, 0, 1 };
}

Quaternion Quaternion::AngleAxis(const float angle, const Vector3 axis)
{
	Vector3 v = axis;
	v.Normalize();
	v *= sinf(angle / 360 * Math::pi);

	return Quaternion{ v.x, v.y, v.z, cosf(angle / 360 * Math::pi) };
}

//assumes rotation sequencing of yaw, pitch, then roll, or Body 3-2-1
Quaternion Quaternion::EulerAnglesToQuaternion(Vector3 eulerAngles)
{
	eulerAngles *= Math::pi / 180.f; // conversion to radians

	float cosY = cosf(eulerAngles.z * 0.5f);
	float sinY = sinf(eulerAngles.z * 0.5f);
	float cosP = cosf(eulerAngles.y * 0.5f);
	float sinP = sinf(eulerAngles.y * 0.5f);
	float cosR = cosf(eulerAngles.x * 0.5f);
	float sinR = sinf(eulerAngles.x * 0.5f);

	return Quaternion
	{
	  sinR * cosP * cosY - cosR * sinP * sinY,
	  cosR * sinP * cosY + sinR * cosP * sinY,
	  cosR * cosP * sinY - sinR * sinP * cosY,
	  cosR * cosP * cosY + sinR * sinP * sinY
	};
}

Quaternion Quaternion::EulerAnglesToQuaternion(float x, float y, float z)
{
	return EulerAnglesToQuaternion(Vector3(x, y, z));
}

Vector3 Quaternion::QuaternionToEulerAngles(Quaternion q)
{
	Vector3 eulerAngles;

	// roll (x-axis rotation)
	float sinRcosP = 2 * (q.w * q.x + q.y * q.z);
	float cosRcosP = 1 - 2 * (q.x * q.x + q.y * q.y);
	eulerAngles.x = std::atan2f(sinRcosP, cosRcosP);

	// pitch (y-axis rotation)
	float sinP = 2 * (q.w * q.y - q.z * q.x);
	if (std::abs(sinP) >= 1)
		eulerAngles.y = std::copysign(Math::pi / 2, sinP); // use 90 degrees if out of range
	else
		eulerAngles.y = std::asinf(sinP);

	// yaw (z-axis rotation)
	float sinYcosP = 2 * (q.w * q.z + q.x * q.y);
	float cosYcosP = 1 - 2 * (q.y * q.y + q.z * q.z);
	eulerAngles.z = std::atan2f(sinYcosP, cosYcosP);

	eulerAngles *= 180.f / Math::pi; // conversion to degrees

	return eulerAngles;
}

Quaternion Quaternion::operator*=(Quaternion rhs)
{
	Quaternion lhs = *this;

	x = lhs.x * rhs.w + lhs.w * rhs.x + lhs.y * rhs.z - lhs.z * rhs.y;
	y = lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x;
	z = lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w;
	w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;

	return *this;
}

Quaternion operator*(Quaternion lhs, Quaternion rhs)
{
	return lhs *= rhs;
}