#include "Transform.h"
#include "Math.h"
#include <iostream>

Transform::Transform(Vector3 pos, Quaternion rot, Vector3 sca)
	: position{ pos }, rotation{ rot }, scale{ sca } {}

Matrix4x4 Transform::LocalToWorldMatrix()
{
	return Matrix4x4::Transpose(Matrix4x4::Translate(position) * (Matrix4x4::Rotate(rotation) * Matrix4x4::Scale(scale)));
}

Vector3 Transform::Forward() const
{
	return Vector3(
		2 * rotation.x * rotation.z + 2 * rotation.w * rotation.y,
		2 * rotation.y * rotation.z - 2 * rotation.w * rotation.x,
		2 * rotation.w * rotation.w + 2 * rotation.z * rotation.z - 1);
}

Vector3 Transform::Back() const
{
	return -Forward();
}

Vector3 Transform::Left() const
{
	return Vector3(
		2 * rotation.w * rotation.w + 2 * rotation.x * rotation.x - 1,
		2 * rotation.x * rotation.y + 2 * rotation.w * rotation.z,
		2 * rotation.x * rotation.z - 2 * rotation.w * rotation.y);
}

Vector3 Transform::Right() const
{
	return -Left();
}

Vector3 Transform::Up() const
{
	return Vector3(
		2 * rotation.x * rotation.y - 2 * rotation.w * rotation.z,
		2 * rotation.w * rotation.w + 2 * rotation.y * rotation.y - 1,
		2 * rotation.y * rotation.z + 2 * rotation.w * rotation.x);
}

Vector3 Transform::Down() const
{
	return -Up();
}