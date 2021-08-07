#pragma once

#include "../network/NetworkSystem.h"

#include "../input/InputSystem.h"
#include "../window/WindowSystem.h"
#include "../physics/PhysicSystem.h"
#include "../logic/LogicSystem.h"
#include "../graphics/GraphicsSystem.h"
#include "../Tools/Timer.h"
#include "../Tools/CommandLine_Parser.h"
#include "Factory.h"

#include <Windows.h>


class Game
{
public:
	Game(HINSTANCE hinstance, 
		int nCmdShow, 
		unsigned width, 
		unsigned height, 
		Hostname_Port_List& list);
	bool GameIsRunning() const;

	void Run();


private:
	InputSystem _inputSystem;
	WindowSystem _windowSystem;
	PhysicSystem _physicSystem;
	LogicSystem  _logicSystem;
	GraphicsSystem _graphicsSystem;
	Factory * _factory;

//the network system runs multiple threads
//to receive packets from socket 
//from each address
	NetworkSystem _networkSystem;

	Timer _gametime;
	Timer _dt;
	static constexpr float dt = 1.0f / 60.0f;



	bool _isGameRunning;

};