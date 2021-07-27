#pragma once

#include "../physics/Transform.h"
#include "../physics/Rigidbody.h"
#include "../physics/AABBCollider.h"

class GameObject
{
public:

private:
	Transform _transform;
	Rigidbody _rigidbody;
	AABBCollider _aabb;
};