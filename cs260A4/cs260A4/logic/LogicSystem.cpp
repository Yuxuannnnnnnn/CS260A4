#include "LogicSystem.h"
#include <cmath>
#include "../core/GameObject.h"
#include "../physics/Vector2.h"

void LogicSystem::Update(const InputSystem& inputsystem, float dt)
{
	// find Ship belong to this client
	GameObject* ship = nullptr;

	if (inputsystem.KeyHold(VK_W))
	{
		Vector2 accel{ cosf(ship->transform.rotation) * acceleration_speed * dt,
							 sinf(ship->transform.rotation) * acceleration_speed * dt };

		ship->rigidbody.acceleration = accel;

		// broadcast this acceleration to all other client

		// end broadcast

		ship->rigidbody.velocity = ship->rigidbody.velocity + accel;

		// (from CS230) scale velocity by 0.99 to simulate drag and prevent velocity out of control
		ship->rigidbody.velocity = ship->rigidbody.velocity * 0.99f;
	}

	if (inputsystem.KeyHold(VK_S))
	{
		Vector2 accel{ cosf(ship->transform.rotation) * -acceleration_speed * dt,
							 sinf(ship->transform.rotation) * -acceleration_speed * dt };

		ship->rigidbody.acceleration = accel;

		// broadcast this acceleration to all other client

		// end broadcast

		ship->rigidbody.velocity = ship->rigidbody.velocity + accel;

		// (from CS230) scale velocity by 0.99 to simulate drag and prevent velocity out of control
		ship->rigidbody.velocity = ship->rigidbody.velocity * 0.99f;
	}

	if (inputsystem.KeyHold(VK_A))
	{
		ship->transform.rotation += rotation_speed * dt;
		ship->transform.rotation = Wrap(ship->transform.rotation, -PI, PI);

		// broadcast this acceleration to all other client

		// end broadcast
	}

	if (inputsystem.KeyHold(VK_D))
	{
		ship->transform.rotation -= rotation_speed * dt;
		ship->transform.rotation = Wrap(ship->transform.rotation, -PI, PI);

		// broadcast this acceleration to all other client

		// end broadcast
	}
}

float LogicSystem::Wrap(float x, float x0, float x1)
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