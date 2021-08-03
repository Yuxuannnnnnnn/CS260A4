#include "Game.h"

Game::Game(HINSTANCE hinstance, int nCmdShow, unsigned width, unsigned height):
	_windowSystem{hinstance, nCmdShow, width, height},
	_isGameRunning{true}
{
}



//check boolean in main.cpp
bool Game::GameIsRunning() const
{
	return _isGameRunning;
}


//update loop - per frame
void Game::Update()
{
	_inputSystem.Update();
	_logicSystem.Update(_inputSystem, 0.016f);
	_physicSystem.Update();

	_windowSystem.Update(_isGameRunning);
}
