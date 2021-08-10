#pragma once

#include "../physics/Transform.h"
#include "../physics/Rigidbody.h"
#include "../physics/AABBCollider.h"

#include "../Math/Vector.h"

enum class TYPE
{
	TYPE_PLAYER = 0,
	TYPE_BULLET,
	TYPE_ASTEROID,
	TYPE_NUM
};



enum class MeshType
{
	quad,
	triangle
};


struct GameObject
{
	GameObject()
	{

	}

	GameObject(const Transforms& trans, const Rigidbody& rb, TYPE objtype, MeshType meshtype = MeshType::quad, int playerindex = -1)
	{
		transform = trans;
		rigidbody = rb;
		obj_type = objtype;
		mesh = meshtype;
		playerIndex = playerindex;
		
	}

	~GameObject()
	{

	}

	Transforms transform; //will use this for graphics
	Rigidbody rigidbody;
	AABBCollider aabb;  //all asteroids will be the same size collider
	

	MeshType mesh{ MeshType::quad};
	Vector3 color;
	
	TYPE obj_type{ TYPE::TYPE_NUM };
	int playerIndex{ -1 };

	// test code, remove later

	bool isActive = true;
};