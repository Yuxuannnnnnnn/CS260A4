#pragma once
#include "AABBCollider.h"
#include "../core/GameObject.h"
#include <vector>
#include "../core/Factory.h"
class PhysicSystem
{
public:
	void Update(Factory* factory);
	void TestUpdate(std::vector<GameObject>& objlist, float dt)
	{
		for (auto& gameobj : objlist)
		{
			if (gameobj.isActive == false)
				continue;

			gameobj.transform.position = gameobj.transform.position + gameobj.rigidbody.velocity * dt;

			gameobj.aabb.min = { gameobj.transform.position.x - 0.5f * gameobj.transform.scale.x, gameobj.transform.position.y - 0.5f * gameobj.transform.scale.y };
			gameobj.aabb.max = { gameobj.transform.position.x + 0.5f * gameobj.transform.scale.x, gameobj.transform.position.y + 0.5f * gameobj.transform.scale.y };

			if (gameobj.obj_type == TYPE::TYPE_BULLET)
				continue;

			gameobj.transform.position.x = Wrap(gameobj.transform.position.x, -400, 400);
			gameobj.transform.position.y = Wrap(gameobj.transform.position.y, -300, 300);

		}

		for (auto& gameobj : objlist)
		{
			if (gameobj.isActive == false)
				continue;

			if (gameobj.obj_type == TYPE::TYPE_BULLET)
			{
				for (auto& gameobj2 : objlist)
				{
					if (gameobj2.obj_type == TYPE::TYPE_ASTEROID)
					{
						if (CollisionIntersection(gameobj.aabb, gameobj2.aabb))
						{
							gameobj2.isActive = false;
							gameobj.isActive = false;
						}
					}
				}
			}

		}
	}
private:

	float Wrap(float x, float x0, float x1)
	{
		float range = x1 - x0;
		if (x < x0)
		{
			return x0 + range;
		}
		if (x > x1)
		{
			return x1 - range;
		}
		return x;
	}

	void UpdateTransform(GameObject& gameobj, float dt);
	bool CollisionIntersection(const AABBCollider& aabb1, const AABBCollider& aabb2);
};