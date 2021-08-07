#pragma once

#include "Vector2.h"

struct Rigidbody
{
	Rigidbody() = default;
	Rigidbody(const Vector2& vel)
	{
		velocity = vel;
	}
	Vector2 velocity;
	Vector2 acceleration;
};