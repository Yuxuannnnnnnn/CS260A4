#include "Game.h"
#include <iostream>
#include <thread>
#include <functional>
#include <iostream>



Game::Game(HINSTANCE hinstance, int nCmdShow, unsigned width, unsigned height, 
	Hostname_Port_List& list) :
	_windowSystem{ hinstance, nCmdShow, width, height },
	_isGameRunning{ true }
{
	_factory = new Factory{};

	//graphics initialisation
	_graphicsSystem.Init(_windowSystem.GetHandle());

	_networkSystem.Init(
		list,
		std::bind(&LogicSystem::InsertEvent, 
		&_logicSystem, 
		std::placeholders::_1, 
		std::placeholders::_2));


	_logicSystem.Init(
		_factory,
		std::bind(&NetworkSystem::InsertNotification,
			&_networkSystem,
			std::placeholders::_1,
			std::placeholders::_2, 
			std::placeholders::_3), 
		list.size());

	_gametime.Start();
}

//check boolean in main.cpp
bool Game::GameIsRunning() const
{
	return _isGameRunning;
}

//update loop - per frame
void Game::Run()
{

	//wait for all clients to be online first
	//before starting game

#define Test
#ifdef Test

	//the network system runs a seperate thread
	//to receive messages from socket 
	//from each address
	std::thread NetworkSystem_Thread{
			&NetworkSystem::ReceiveEventsFromClient,
			std::ref(_networkSystem) };

#endif


	_logicSystem.Wait_ForAllPlayers();
	_logicSystem.HostInitGame();

	while (GameIsRunning())
	{
		//reset the timer
		_dt.Reset();

		_windowSystem.Update(_isGameRunning);		//Go through all Windows Messages

		_inputSystem.Update();						//get inputs
		_logicSystem.Update(
			_inputSystem, 
			dt, _gametime.GetDuration(), _factory);  //check player logic in input
		
		
		_physicSystem.Update(_factory);						//update physics

		_graphicsSystem.Update(_factory);					//update graphics
		_graphicsSystem.LateUpdate();				//update graphics - swap buffer

		//Update factory at the end of loop
		//delete all the object in the deletionlist
		_factory->update();

		//waste time to make it 60 frames per second
		while (_dt.GetDuration() < dt)
		{
		}

	}

	//std::cout << "delta time is " << _dt.GetDuration() << std::endl;


	NetworkSystem_Thread.join();
}

