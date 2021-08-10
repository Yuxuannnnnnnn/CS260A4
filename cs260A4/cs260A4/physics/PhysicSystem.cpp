#include "PhysicSystem.h"


void PhysicSystem::Update(Factory* factory)
{
	// update gameobject rigidbody and position
	for (auto& pair : factory->gameObjects)
	{
		UpdateTransform(pair.second, 0.016f);
	}
	

	// collision detection
}

void PhysicSystem::UpdateTransform(GameObject& gameobj, float dt)
{
	// v1 = a * t + v0
	// pos1 = v1 * t + pos0
	// simulate drag multiply velocity by 0.99
	//gameobj.rigidbody.velocity = gameobj.rigidbody.velocity + gameobj.rigidbody.acceleration * dt;
	gameobj.transform.position = gameobj.transform.position + gameobj.rigidbody.velocity * dt;
	gameobj.aabb.min = { gameobj.transform.position.x - 0.5f * gameobj.transform.scale.x, gameobj.transform.position.y - 0.5f * gameobj.transform.scale.y };
	gameobj.aabb.max = { gameobj.transform.position.x + 0.5f * gameobj.transform.scale.x, gameobj.transform.position.y + 0.5f * gameobj.transform.scale.y };

	if (gameobj.obj_type == TYPE::TYPE_BULLET)
		return;

	gameobj.transform.position.x = Wrap(gameobj.transform.position.x, -400, 400);
	gameobj.transform.position.y = Wrap(gameobj.transform.position.y, -300, 300);

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