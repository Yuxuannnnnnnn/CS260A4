#pragma once

#include "../physics/Transform.h"
#include "../physics/Rigidbody.h"
#include "../physics/AABBCollider.h"

struct GameObject
{
	Transform transform; //will use this for graphics
	Rigidbody rigidbody;
	AABBCollider aabb;  //all asteroids will be the same size collider
};