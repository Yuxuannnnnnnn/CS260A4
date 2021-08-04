#include "Game.h"
#include <iostream>


Game::Game(HINSTANCE hinstance, int nCmdShow, unsigned width, unsigned height) :
	_windowSystem{ hinstance, nCmdShow, width, height },
	_isGameRunning{ true }
{
	//graphics initialisation
	_graphicsSystem.Init(_windowSystem.GetHandle());
	_gametime.Start();
}

//check boolean in main.cpp
bool Game::GameIsRunning() const
{
	return _isGameRunning;
}

//update loop - per frame
void Game::Update()
{
	_dt.Reset();
	_windowSystem.Update(_isGameRunning);		//Go through all Windows Messages

	_inputSystem.Update();						//get inputs
	_logicSystem.Update(_inputSystem, 0.016f);  //check player logic in input
	_physicSystem.Update();						//update physics

	_graphicsSystem.Update();					//update graphics
	_graphicsSystem.LateUpdate();					//update graphics

	while (_dt.GetDuration() < dt)
	{
	}

	//std::cout << "delta time is " << _dt.GetDuration() << std::endl;
}