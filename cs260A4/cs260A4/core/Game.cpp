#include "Game.h"

Game::Game(HINSTANCE hinstance, int nCmdShow, unsigned width, unsigned height):
	_windowSystem{hinstance, nCmdShow, width, height},
	_isGameRunning{true}
{
}

bool Game::GameIsRunning() const
{
	return _isGameRunning;
}

void Game::Update()
{
	_inputSystem.Update();

	_windowSystem.Update(_isGameRunning);
}
