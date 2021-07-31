#pragma once

#include "../physics/Transform.h"
#include "../physics/Rigidbody.h"
#include "../physics/AABBCollider.h"

struct GameObject
{
	Transform transform;
	Rigidbody rigidbody;
	AABBCollider aabb;
};