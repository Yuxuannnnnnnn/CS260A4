#include "Game.h"

Game::Game(HINSTANCE hinstance, int nCmdShow, unsigned width, unsigned height) :
	_windowSystem{ hinstance, nCmdShow, width, height },
	_isGameRunning{ true }
{
	//graphics initialisation
	_graphicsSystem.Init(_windowSystem.GetHandle());
}

//check boolean in main.cpp
bool Game::GameIsRunning() const
{
	return _isGameRunning;
}

//update loop - per frame
void Game::Update()
{
	_windowSystem.Update(_isGameRunning);		//Go through all Windows Messages

	_inputSystem.Update();						//get inputs
	_logicSystem.Update(_inputSystem, 0.016f);  //check player logic in input
	_physicSystem.Update();						//update physics

	_graphicsSystem.Update();					//update graphics
	_graphicsSystem.LateUpdate();					//update graphics
}