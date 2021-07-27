#pragma once

#include <Windows.h>
#include "../input/InputSystem.h"
#include "../window/WindowSystem.h"
#include "../physics/PhysicSystem.h"

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
	bool _isGameRunning;

};