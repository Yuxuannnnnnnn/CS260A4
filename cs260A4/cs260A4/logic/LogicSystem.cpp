#include "LogicSystem.h"
#include <cmath>
#include "../core/GameObject.h"
#include "../physics/Vector2.h"
#include "../network/GameCommands.h"
#include <winsock.h>






void LogicSystem::Update(const InputSystem& inputsystem, float dt, float gametime, Factory* factory)
{


	// get the reference of Ship belong to this client
	GameObject& ownship = factory->getOwnPlayer();

	/*
		From CS230:
	*/

	// This input handling moves the ship without any velocity nor acceleration
	// It should be changed when implementing the Asteroids project
	//
	// Updating the velocity and position according to acceleration is 
	// done by using the following:
	// Pos1 = 1/2 * a*t*t + v0*t + Pos0
	//
	// In our case we need to divide the previous equation into two parts in order 
	// to have control over the velocity and that is done by:
	//
	// v1 = a*t + v0		//This is done when the UP or DOWN key is pressed 
	// Pos1 = v1*t + Pos0

	if (inputsystem.KeyHold(VK_W))
	{
		Vector2 accel{ cosf(ownship.transform.rotation) * acceleration_speed * dt,
							 sinf(ownship.transform.rotation) * acceleration_speed * dt };

		factory->getOwnPlayer().rigidbody.acceleration = accel;

		// broadcast this acceleration to all other client



		// _playerID is the global player index

		// convert everything to string

		std::string drdatastring;

		// size of 

		//DRData drdata{ (float)htonl(accel.x), (float)htonl(accel.y), htonl(gametime), htonl(_playerID) };


	/*	_InsertNotification(GameCommands::MoveForward,
			{ {(char*)&drdata, sizeof(DRData)} },
			-1);*/

			// end broadcast
		ownship.rigidbody.velocity = ownship.rigidbody.velocity + accel;

		// (from CS230) scale velocity by 0.99 to simulate drag and prevent velocity out of control
		ownship.rigidbody.velocity = ownship.rigidbody.velocity * 0.99f;
	}

	if (inputsystem.KeyHold(VK_S))
	{
		Vector2 accel{ cosf(ownship.transform.rotation) * -acceleration_speed * dt,
							 sinf(ownship.transform.rotation) * -acceleration_speed * dt };

		ownship.rigidbody.acceleration = accel;


		// playerID is the global ID 

		DRData drdata{ accel.x, accel.y, gametime, _playerID };

		MessageList messageList;
		InsertDRData_MessageList(messageList, drdata);

		// -1 is broadcast
		_InsertNotification(GameCommands::MoveForward, { messageList }, -1);


		ownship.rigidbody.velocity = ownship.rigidbody.velocity + accel;

		// (from CS230) scale velocity by 0.99 to simulate drag and prevent velocity out of control
		ownship.rigidbody.velocity = ownship.rigidbody.velocity * 0.99f;
	}

	if (inputsystem.KeyHold(VK_A))
	{
		ownship.transform.rotation += rotation_speed * dt;
		ownship.transform.rotation = Wrap(ownship.transform.rotation, -PI, PI);

		// broadcast this acceleration to all other client
		_InsertNotification(GameCommands::RotateLeft,
			{ {(char*)&ownship.transform.rotation, sizeof(ownship.transform.rotation)} },
			-1);
		// end broadcast
	}

	if (inputsystem.KeyHold(VK_D))
	{
		ownship.transform.rotation -= rotation_speed * dt;
		ownship.transform.rotation = Wrap(ownship.transform.rotation, -PI, PI);

		// broadcast this acceleration to all other client
		_InsertNotification(GameCommands::RotateRight,
			{ {(char*)&ownship.transform.rotation, sizeof(ownship.transform.rotation)} },
			-1);
		// end broadcast
	}

	PullEvent(gametime, factory);

	_loopCounter++;
	if (_loopCounter >= _synCount)
	{
		_loopCounter = 0;
		SynchronisePosition();
	}
}

void LogicSystem::TestUpdate(const InputSystem& inputsystem, float dt, std::vector<GameObject>& gameobjlist)
{

	for (auto& gameobj : gameobjlist)
	{
		if (gameobj.playerIndex == 0)
		{
			if (inputsystem.KeyHold(VK_W))
			{
				Vector2 accel{ cosf(gameobj.transform.rotation) * acceleration_speed * dt,
									 sinf(gameobj.transform.rotation) * acceleration_speed * dt };

				gameobj.rigidbody.velocity = gameobj.rigidbody.velocity + accel;

				// (from CS230) scale velocity by 0.99 to simulate drag and prevent velocity out of control
				gameobj.rigidbody.velocity = gameobj.rigidbody.velocity * 0.99f;

			}

			if (inputsystem.KeyHold(VK_S))
			{
				Vector2 accel{ cosf(gameobj.transform.rotation) * -acceleration_speed * dt,
									 sinf(gameobj.transform.rotation) * -acceleration_speed * dt };

				gameobj.rigidbody.velocity = gameobj.rigidbody.velocity + accel;

				// (from CS230) scale velocity by 0.99 to simulate drag and prevent velocity out of control
				gameobj.rigidbody.velocity = gameobj.rigidbody.velocity * 0.99f;

			}

			if (inputsystem.KeyHold(VK_A))
			{
				gameobj.transform.rotation += rotation_speed * dt;
				gameobj.transform.rotation = Wrap(gameobj.transform.rotation, -PI, PI);
			}

			if (inputsystem.KeyHold(VK_D))
			{
				gameobj.transform.rotation -= rotation_speed * dt;
				gameobj.transform.rotation = Wrap(gameobj.transform.rotation, -PI, PI);
			}

			if (inputsystem.KeyPressed(VK_SPACEBAR))
			{
				float bulletspeed = 200.0f;
				GameObject bullet;
				bullet.transform.position = gameobj.transform.position;
				bullet.transform.rotation = gameobj.transform.rotation;
				bullet.transform.scale = { 15.0f, 3.0f };
				bullet.obj_type = TYPE_BULLET;
				bullet.rigidbody.velocity = { cosf(gameobj.transform.rotation) * bulletspeed, sinf(gameobj.transform.rotation) * bulletspeed };
				gameobjlist.push_back(bullet);
			}
		}
	}


}

void LogicSystem::Lock_Step(GameCommands gamecommands)
{
	committment = Hash(gamecommands);

	// broadcast the commitment

	// get everyone else commitments ( other people also broadcasting)

	// wait till everyone ready

	// annouce action

	// validate action

}

void* LogicSystem::Hash(GameCommands gamecommands)
{
	return nullptr;
}

void LogicSystem::PullEvent(float currgametime, Factory* factory)
{

	std::lock_guard<std::mutex> EventsListLock{ EventsList_Mutex };
	for (int i = 0; i < EventsList.size(); i++)
	{
		auto& events = EventsList[i];
		clientsAddressIndex clientAddrIndex = events.first;
		Event event = events.second;
		GameCommands command = event.first;
		MessageList messageList = event.second;

		if (command == GameCommands::SynchronisePlayer)
		{
			//get number of GameObjects
			int NumberOfGameObjects = 0;
			Extract_Number_MessageList(0, messageList, NumberOfGameObjects);
			for (size_t i = 0; i < NumberOfGameObjects; i++)
			{
				int gameObjectID = 0;
				Extract_Number_MessageList(i * 14 + 1, messageList, gameObjectID);

				GameObject object;
				ExtractGameObject_MessageList(i * 14 + 2, messageList, object);
				factory->insertGameObject(gameObjectID, object);

				factory->insertPlayerID_GameObject(object.playerIndex, gameObjectID);
			}

		}
		else if (command == GameCommands::SyncrhoniseAsteroids)
		{
			//get number of GameObjects
			int NumberOfGameObjects = 0;
			Extract_Number_MessageList(0, messageList, NumberOfGameObjects);
			for (size_t i = 0; i < NumberOfGameObjects; i++)
			{
				int gameObjectID = 0;
				Extract_Number_MessageList(i * 14 + 1, messageList, gameObjectID);

				GameObject object;
				ExtractGameObject_MessageList(i * 14 + 2, messageList, object);
				factory->insertGameObject(gameObjectID, object);
			}
		}
		// when player move forward
		else if (command == GameCommands::MoveForward)
		{
			DRData drdata;
			ExtractDRData_MessageList(0, messageList, drdata);
			PerformDR(factory->getPlayer(drdata.playerindex), drdata, currgametime);
		}

		else if (command == GameCommands::MoveBackward)
		{

			DRData drdata;
			ExtractDRData_MessageList(0, messageList, drdata);
			PerformDR(factory->getPlayer(drdata.playerindex), drdata, currgametime);
			/*DRData drdata;
			memcpy(&drdata, &(event.second[0]), sizeof(DRData));
			drdata.accelx = ntohl(drdata.accelx);
			drdata.accely = ntohl(drdata.accely);
			drdata.gametime = ntohl(drdata.gametime);
			drdata.playerindex = ntohl(drdata.playerindex);

			PerformDR(factory->getPlayer(clientAddrIndex), drdata, currgametime);*/
		}

		else if (command == GameCommands::RotateLeft)
		{
			float receivedRotation;
			Extract_Number_MessageList(0, messageList, receivedRotation);
			factory->getPlayer(clientAddrIndex).transform.rotation = receivedRotation;
		}
		else if (command == GameCommands::RotateRight)
		{
			float receivedRotation;
			Extract_Number_MessageList(0, messageList, receivedRotation);
			factory->getPlayer(clientAddrIndex).transform.rotation = receivedRotation;
		}

	}
	EventsList.clear();

	// while event list not empty, pull events
	//for (auto& eventpair : EventsList)
	//{
	//	auto command = eventpair.first;
	//	switch (command)
	//	{	
	//	case (int)GameCommands::MoveForward:
	//		DRData drdata;
	//		playerIndex index = eventpair.first;
	//		memcpy(&drdata, &(eventpair.second[0]), sizeof(DRData));

	//		// find the correct player index from Gameobj container from factory

	//		GameObject* player = nullptr;



	//		break;
	//	case (int)GameCommands::MoveBackward:
	//		DRData drdata;
	//		memcpy(&drdata, &(eventpair.second[0]), sizeof(DRData));

	//		// find the correct player index from Gameobj container from factory

	//		GameObject* player = nullptr;
	//		PerformDR(player, currgametime, drdata.gametime, Vector2{ drdata.accelx, drdata.accely });


	//		break;
	//	case (int)GameCommands::RotateLeft:



	//	}

	//}

	//EventsList.clear();
}

void LogicSystem::SynchronisePosition()
{

	// send a syn command, of player position, rotation, velocity
}

void LogicSystem::PerformDR(GameObject& ship, const DRData& drdata, float currgametime)
{
	// receive a ship player to perform DR
	// receive the game time of the sender
	// find the time difference, game time on this client - game time of sender
	// calculate the DR position
	float timediff = currgametime - drdata.gametime;


	ship.rigidbody.velocity = ship.rigidbody.velocity + Vector2{ drdata.accelx, drdata.accely } *timediff;

	// extrapolate the position base on time difference
	ship.transform.position = ship.transform.position + ship.rigidbody.velocity * timediff;

	// CS230 : stimulate drag and prevent velocity go out of control
	ship.rigidbody.velocity = ship.rigidbody.velocity * 0.99f;

}

float LogicSystem::Wrap(float x, float x0, float x1)
{
	float range = x1 - x0;
	if (x < x0)
	{
		return x0 + range;
	}
	if (x > x1)
	{
		return x1 - range;
	}
	return x;
}