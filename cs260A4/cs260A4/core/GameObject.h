#pragma once

#include "../physics/Transform.h"
#include "../physics/Rigidbody.h"
#include "../physics/AABBCollider.h"

#include "../Math/Vector.h"

enum TYPE
{
	TYPE_PLAYER = 0,
	TYPE_BULLET,
	TYPE_ASTEROID,
	TYPE_NUM
};



enum MeshType
{
	quad,
	triangle
};


struct GameObject
{
	GameObject() = default;

	GameObject(const Transforms& trans, const Rigidbody& rb, TYPE objtype, MeshType meshtype = quad, int playerindex = -1)
	{
		transform = trans;
		rigidbody = rb;
		obj_type = objtype;
		mesh = meshtype;
		playerIndex = playerindex;
		
	}

	~GameObject() = default;

	Transforms transform; //will use this for graphics
	Rigidbody rigidbody;
	AABBCollider aabb;  //all asteroids will be the same size collider
	

	MeshType mesh{quad};
	Vector3 color;
	
	TYPE obj_type;
	int playerIndex{ -1 };

	// test code, remove later

	bool isActive = true;
};