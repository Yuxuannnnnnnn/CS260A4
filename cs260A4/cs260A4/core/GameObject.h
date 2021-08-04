#pragma once

#include "../physics/Transform.h"
#include "../physics/Rigidbody.h"
#include "../physics/AABBCollider.h"


enum TYPE
{
	TYPE_PLAYER = 0,
	TYPE_BULLET,
	TYPE_ASTEROID,
	TYPE_NUM
};

struct GameObject
{
	Transforms transform; //will use this for graphics
	Rigidbody rigidbody;
	AABBCollider aabb;  //all asteroids will be the same size collider
	TYPE obj_type;
};