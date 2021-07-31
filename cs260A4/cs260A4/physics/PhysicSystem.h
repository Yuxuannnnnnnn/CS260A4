#pragma once
#include "AABBCollider.h"
#include "../core/GameObject.h"

class PhysicSystem
{
public:
	void Update();
private:
	void UpdateTransform(GameObject* gameobj, float dt);
	bool CollisionIntersection(const AABBCollider& aabb1, const AABBCollider& aabb2);
};