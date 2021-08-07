#pragma once

#include "Vector2.h"

struct Transforms
{
	Transforms() = default;
	Transforms(const Vector2& pos, float rot, const Vector2& scal)
	{
		position = pos;
		rotation = rot;
		scale = scal;
	}
	Vector2 position;
	float rotation = 0;
	Vector2 scale;
};