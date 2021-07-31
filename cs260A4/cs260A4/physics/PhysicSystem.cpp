#include "PhysicSystem.h"

void PhysicSystem::Update()
{
	// update gameobject rigidbody and position

	// collision detection

}

void PhysicSystem::UpdateTransform(GameObject* gameobj, float dt)
{
	// v1 = a * t + v0
	// pos1 = v1 * t + pos0
	// simulate drag multiply velocity by 0.99
	gameobj->rigidbody.velocity = gameobj->rigidbody.velocity + gameobj->rigidbody.acceleration * dt;
	gameobj->transform.position = gameobj->transform.position + gameobj->rigidbody.velocity * dt;
	gameobj->rigidbody.velocity = gameobj->rigidbody.velocity * 0.99f;
}

bool PhysicSystem::CollisionIntersection(const AABBCollider& aabb1, const AABBCollider& aabb2)
{
	// non-dynamic AABB collision

	if (aabb1.max.x < aabb2.min.x || aabb1.min.x > aabb2.max.x)
		return false;

	if (aabb1.max.y < aabb2.min.y || aabb1.min.y > aabb2.max.y)
		return false;

	return true;
}
