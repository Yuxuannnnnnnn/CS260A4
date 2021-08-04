#pragma once

#include <Windows.h>
#include "../input/InputSystem.h"
#include "../window/WindowSystem.h"
#include "../physics/PhysicSystem.h"
#include "../logic/LogicSystem.h"
#include "../logic/LogicSystem.h"
#include "../graphics/GraphicsSystem.h"
#include "../Tools/Timer.h"

class Game
{
public:
	Game(HINSTANCE hinstance, int nCmdShow, unsigned width, unsigned height);
	bool GameIsRunning() const;

	void Update();

private:
	InputSystem _inputSystem;
	WindowSystem _windowSystem;
	PhysicSystem _physicSystem;
	LogicSystem  _logicSystem;
	GraphicsSystem _graphicsSystem;
	Timer _gametime;
	Timer _dt;
	static constexpr float dt = 1.0f / 60.0f;
	bool _isGameRunning;
};