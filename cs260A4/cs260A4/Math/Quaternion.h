#pragma once

struct Vector3;

struct Quaternion
{
public:
	float x, y, z, w;

	Quaternion();
	Quaternion(float, float, float, float);
	static const Quaternion Identity();
	static Quaternion AngleAxis(float angle, Vector3 axis);

	//assumes rotation sequencing of yaw, pitch, then roll, or Body 3-2-1
	static Quaternion EulerAnglesToQuaternion(Vector3 eulerAngles);

	static Quaternion EulerAnglesToQuaternion(float x, float y, float z);

	static Vector3 QuaternionToEulerAngles(Quaternion q);

	Quaternion operator*=(Quaternion rhs);
};

Quaternion operator*(Quaternion lhs, Quaternion rhs);
