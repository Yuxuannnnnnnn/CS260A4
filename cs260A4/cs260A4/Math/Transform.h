#pragma once

#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"

struct Transform
{
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
	Transform(
		Vector3 position = Vector3::Zero(),
		Quaternion rotation = Quaternion::Identity(),
		Vector3 scale = Vector3::One());
	Matrix4x4 LocalToWorldMatrix();
	Vector3 Forward() const;
	Vector3 Back() const;
	Vector3 Left() const;
	Vector3 Right() const;
	Vector3 Up() const;
	Vector3 Down() const;
};
