#pragma once

#include <Windows.h>
#include "../input/InputSystem.h"
#include "../window/WindowSystem.h"
#include "../physics/PhysicSystem.h"
#include "../logic/LogicSystem.h"
#include "../graphics/GraphicsSystem.h"
#include "../Tools/Timer.h"

#include <vector>
#include "GameObject.h"
class TestGame
{
public:
	TestGame(HINSTANCE hinstance,
		int nCmdShow,
		unsigned width,
		unsigned height, std::vector<GameObject>& objlist):
		_objlist{objlist},
		_windowSystem{ hinstance, nCmdShow, width, height }
	{
		_graphicsSystem.Init(_windowSystem.GetHandle());
	}

	void Run()
	{
		while (isrunning)
		{
			//reset the timer
			_dt.Reset();

			_windowSystem.Update(isrunning);		//Go through all Windows Messages

			_inputSystem.Update();						//get inputs
			_logicSystem.TestUpdate(
				_inputSystem,
				dt, _objlist);									 //check player logic in input


			//_physicSystem.Update();						//update physics

			_physicSystem.TestUpdate(_objlist, dt);

			_graphicsSystem.Update(_objlist);					//update graphics
			_graphicsSystem.LateUpdate();				//update graphics - swap buffer

	

			//waste time to make it 60 frames per second
			while (_dt.GetDuration() < dt)
			{
			}

		}

		_graphicsSystem.CleanRenderingEnvironment();
	}

	std::vector<GameObject>& _objlist;
	bool isrunning = true;

	Timer _gametime;
	Timer _dt;
	static constexpr float dt = 1.0f / 60.0f;

	InputSystem _inputSystem;
	WindowSystem _windowSystem;
	PhysicSystem _physicSystem;
	LogicSystem  _logicSystem;
	GraphicsSystem _graphicsSystem;
};