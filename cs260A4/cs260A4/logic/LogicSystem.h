#pragma once

#include "../input/InputSystem.h"
#include "../input/KeyEnum.h"

#define PI 3.1415926f

class LogicSystem
{
public:
	void Update(const InputSystem& inputsystem, float dt);

private:
	float Wrap(float x, float x0, float x1);
	float acceleration_speed = 40.0f;
	float rotation_speed = 6.3f;
};