#include "Game.h"
#include <iostream>
#include <thread>
#include <functional>
#include <iostream>



Game::Game(HINSTANCE hinstance, int nCmdShow, unsigned width, unsigned height) :
	_windowSystem{ hinstance, nCmdShow, width, height },
	_isGameRunning{ true }
{
	//graphics initialisation
	_graphicsSystem.Init(_windowSystem.GetHandle());

	_networkSystem.Init(
		std::bind(&LogicSystem::InsertEvent, 
		&_logicSystem, 
		std::placeholders::_1, 
		std::placeholders::_2), 
		std::bind(&LogicSystem::HostPlayer,
			&_logicSystem,
			std::placeholders::_1, 
			std::placeholders::_2,
			std::placeholders::_3));


	_logicSystem.Init(
		std::bind(&NetworkSystem::InsertNotification,
			&_networkSystem,
			std::placeholders::_1,
			std::placeholders::_2));

	_gametime.Start();
}

//check boolean in main.cpp
bool Game::GameIsRunning() const
{
	return _isGameRunning;
}

//update loop - per frame
void Game::Run(Hostname_Port_List& list)
{

	//wait for all clients to be online first
	//before starting game

#define Test
#ifdef Test

	typedef std::vector<int> clientIndicesList;
	clientIndicesList indicesList;
	_networkSystem.Wait_ToConnectAllClients(list);


	//the network system runs multiple threads
	//to receive packets from socket 
	//from each address
	std::thread NetworkSystem_Thread{
			&NetworkSystem::ReceiveEvents,
			std::ref(_networkSystem) };

#endif


	while (GameIsRunning())
	{
		//reset the timer
		_dt.Reset();

		_windowSystem.Update(_isGameRunning);		//Go through all Windows Messages

		_inputSystem.Update();						//get inputs
		_logicSystem.Update(_inputSystem, dt, _gametime.GetDuration());  //check player logic in input
		

		//send notifications to all other clients
		_networkSystem.BroadcastEventsToClients();
		
		_physicSystem.Update();						//update physics

		_graphicsSystem.Update();					//update graphics
		_graphicsSystem.LateUpdate();				//update graphics - swap buffer

		//Update factory at the end of loop
		//delete all the object in the deletionlist
		_factory.update();

		//waste time to make it 60 frames per second
		while (_dt.GetDuration() < dt)
		{
		}

	}

	//std::cout << "delta time is " << _dt.GetDuration() << std::endl;
}

