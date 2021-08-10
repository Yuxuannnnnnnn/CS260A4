#include "LogicSystem.h"
#include <cmath>
#include "../core/GameObject.h"
#include "../physics/Vector2.h"
#include "../network/GameCommands.h"

#include "../window/WindowSystem.h"

// macro to enable/disable synchroise
#define SYNCHRO 1

// number of frames between each synchronise gameobject 
#define SYNCHRO_COUNT 60 // every 1 seconds




void LogicSystem::GameMenu(NetworkSystem* networkSystem)
{
	PRINTOUT("-----------------GameMenu-------------------");
	PRINTOUT("1. Press J to Join Game");
	PRINTOUT("2. Press G to Create GameRoom");
	PRINTOUT("\0");

	while (true)
	{
		std::string userInput;

		
		std::cout << userInput << std::endl;
		//join a game
		if (!userInput.compare("J"))
		{
			PRINTOUT("Key In Host Hostname:Port: ");

			//get Hostname:Port from commandline console
			std::getline(std::cin, userInput);

			//checkUser_HostnamePort
			auto position = userInput.find(":");
			std::string hostName = userInput.substr(0, position);
			std::string Port = userInput.substr(position + 1);


			//can find the hostName & Port from the commandline
			//get the clientAddrIndex of the host
			if (networkSystem->checkUser_HostnamePort(hostName, Port, hostAddrIndex))
			{
				PRINTOUT("Joining Game...");
				_isHost = false;

				break;
			}
			else //cannot find the hostName & Port from the commandline
			{
				PRINTOUT("Invalid Hostname:Port! Please key in  Hostname:Port Again", "\0");
			}
		}
		//Create a game
		else if (!userInput.compare("G"))
		{
			PRINTOUT("Press G to Create GameRoom:");
			//this player is the host
			_isHost = true;

			break;

		}
		else
		{
			PRINTOUT("Invalid Command! Please key in Command Again", "\0");
		}
	}
}

void LogicSystem::Wait_ForAllPlayers(WindowSystem& _windowSystem, InputSystem& inputsystem)
{
	//------------Wait for all Clients to connect----------------------

		//send a packet to all players that this client has just joinedGame
		//all players who received this message should know
		//this player's sockaddr
	_InsertNotification(GameCommands::JoinGame, {}, -1);

	int count_players_in_game = 1; //count this client
	std::vector<clientsAddressIndex> addressIndex;


	//while the number of players in the game has not reached the requirement
	while (count_players_in_game < _NumOfPlayersRequired)
	{
		bool boolean;
		_windowSystem.Update(boolean);
		inputsystem.Update();						//get inputs

		if (inputsystem.KeyPressed(KEY::VK_A))
		{
			//Respond to the client an InGame notification
			_InsertNotification(GameCommands::JoinGame, {}, -1);
			PRINTOUT("SentJoinGame Mess.");
		}


		//make sure the network System is not insert events to the list
		//while logicSystem is accessing the eventsList
		std::lock_guard<std::mutex> EventsListLock{ EventsList_Mutex };
		for (int i = 0; i < EventsList.size(); i++)
		{
			auto& events = EventsList[i];
			clientsAddressIndex clientAddrIndex = events.first;
			Event event = events.second;
			GameCommands command = event.first;

			//wait for clients to respond that they are already inGame
			if (command == GameCommands::InGame)
			{
				//add this event to be deleted
				EventsDeletionList.push_back(i);

				if (std::find(addressIndex.begin(), addressIndex.end(), clientAddrIndex) == addressIndex.end())
				{
					//count the number of players already in the game
					//so that I can assign a playerID to myself at the end
					++_playerID;


					PRINTOUT("ClientAddressIndex: "
						, clientAddrIndex
						, " is in game.");
					PRINTOUT("Player "
						, _playerID - 1
						, " is in game.");

					addressIndex.push_back(clientAddrIndex);

					//increment number of players in the game
					count_players_in_game++;
				}
			}
			//if clients never respond, then wait for them to send a joinedGame notification
			else if (command == GameCommands::JoinGame)
			{
				//add this event to be deleted
				EventsDeletionList.push_back(i);

				if (std::find(addressIndex.begin(), addressIndex.end(), clientAddrIndex) == addressIndex.end())
				{
					//Respond to the client an InGame notification
					_InsertNotification(GameCommands::InGame, {}, clientAddrIndex);

					PRINTOUT("ClientAddressIndex: "
						, clientAddrIndex,
						" has joined game.");
					PRINTOUT("Player "
						, count_players_in_game
						, " has joined game.");

					addressIndex.push_back(clientAddrIndex);

					//increment number of players in the game
					count_players_in_game++;
				}
			}
		}


		clearEventsToBeDeleted();
	}


	//---------------All clients have connected from this point----------------------

		//if the playerID of this client is 0
		//then this client will be the host
		//that define the settings of the asteroids throughout the game& 
		//and the initial settings of all the player ships
	if (_playerID == 0)
	{
		HostPlayer(true);
	}


	//broadcast to everyone their player Index
	_InsertNotification(
		GameCommands::SendIndex, { {(char*)&_playerID, sizeof(_playerID)} }, -1);



	int countPlayersSentIndex = 1; //count this client
	//find out each player's playerID
	//match each player's playerID to their clientAddres
	while (countPlayersSentIndex < _NumOfPlayersRequired)
	{
		//make sure the network System is not insert events to the list
		//while logicSystem is accessing the eventsList
		std::lock_guard<std::mutex> EventsListLock{ EventsList_Mutex };
		for (int i = 0; i < EventsList.size(); i++)
		{
			auto& events = EventsList[i];
			clientsAddressIndex clientAddrIndex = events.first;
			Event event = events.second;
			GameCommands command = event.first;
			MessageList messageList = event.second;

			//wait for clients to respond their clientsAddressIndex
			if (command == GameCommands::SendIndex)
			{
				EventsDeletionList.push_back(i);

				//extract playerID
				int playerID = -1;
				memcpy(&playerID, messageList[0].message, messageList[0].size_);

				//store playerID with reference to their clientAddressID
				clientAddrID_PlayerID_List[clientAddrIndex] = playerID;

				//increment number of players that has made their playerID known
				countPlayersSentIndex++;
			}
		}
		clearEventsToBeDeleted();

	}

	//let factory know yr playerID
	gameFactory->_playerID = _playerID;



	//--------All clients have made known their playerID to every client at this point----------


}



void LogicSystem::Update(const InputSystem& inputsystem, float dt, float gametime, Factory* factory)
{
	PullEvent(gametime, factory);


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

	if (ownship.playerIndex >= 0)
	{
		/*std::cout << "Factory PID: " << factory->_playerID << ","
				<< "SHIP PID: "  << ownship.playerIndex << std::endl;*/

		if (inputsystem.KeyHold(VK_W))
		{
			Vector2 accel{ cosf(ownship.transform.rotation) * acceleration_speed,
								 sinf(ownship.transform.rotation) * acceleration_speed};

			factory->getOwnPlayer().rigidbody.acceleration = accel;

			// broadcast this acceleration to all other client


			DRData drdata{ accel.x, accel.y, gametime, _playerID };// playerID is the global ID 
			MessageList messageList;
			InsertDRData_MessageList(messageList, drdata);

			// -1 is broadcast
			_InsertNotification(GameCommands::MoveForward, { messageList }, -1);


			ownship.rigidbody.velocity = ownship.rigidbody.velocity + accel * dt;
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
			_InsertNotification(GameCommands::MoveBackward, { messageList }, -1);


			ownship.rigidbody.velocity = ownship.rigidbody.velocity + accel * dt;

			// (from CS230) scale velocity by 0.99 to simulate drag and prevent velocity out of control
			ownship.rigidbody.velocity = ownship.rigidbody.velocity * 0.99f;
		}

		if (inputsystem.KeyHold(VK_A))
		{
			ownship.transform.rotation += rotation_speed * dt;
			ownship.transform.rotation = Wrap(ownship.transform.rotation, -PI, PI);

			MessageList messageList;
			Insert_Number_MessageList(messageList, ownship.transform.rotation);
			// broadcast this acceleration to all other client
			_InsertNotification(GameCommands::RotateLeft,
				{ messageList },
				-1);
			// end broadcast
		}

		if (inputsystem.KeyHold(VK_D))
		{
			ownship.transform.rotation -= rotation_speed * dt;
			ownship.transform.rotation = Wrap(ownship.transform.rotation, -PI, PI);

			MessageList messageList;
			Insert_Number_MessageList(messageList, ownship.transform.rotation);
			// broadcast this acceleration to all other client
			_InsertNotification(GameCommands::RotateRight,
				{ messageList },
				-1);


			// end broadcast
		}

		if (inputsystem.KeyPressed(VK_SPACEBAR))
		{
			// spawn bullet
			Vector2 bulletvelocity = { cosf(ownship.transform.rotation) * bulletspeed, sinf(ownship.transform.rotation) * bulletspeed };
			factory->CreateBullet(ownship.transform.position, ownship.transform.rotation, bulletvelocity, ownship.playerIndex);

			MessageList messageList;
			Insert_Number_MessageList(messageList, ownship.transform.position.x);
			Insert_Number_MessageList(messageList, ownship.transform.position.y);
			Insert_Number_MessageList(messageList, ownship.transform.rotation);
			Insert_Number_MessageList(messageList, bulletvelocity.x);
			Insert_Number_MessageList(messageList, bulletvelocity.y);
			Insert_Number_MessageList(messageList, ownship.playerIndex);
			_InsertNotification(GameCommands::Shoot,
				{ messageList },
				-1);


		}
		if (_isHost)
		{
			CheckCollision(factory);
		}
#if SYNCHRO
		_loopCounter++;
		if (_loopCounter >= SYNCHRO_COUNT)
		{
			_loopCounter = 0;
			SynchronisePosition(factory);
		}
#endif
	}

}


void LogicSystem::SynchronisePosition(Factory* factory)
{

	// synchro own ship gameobject
	MessageList messageList;
	GameObject object = factory->getOwnPlayer();
	InsertGameObject_MessageList(messageList, object);

	_InsertNotification(GameCommands::Synchronise_Player,
		{ messageList },
		-1);

	messageList.clear();

	// host synchro asteroid velocity and position
	if (_isHost)
	{
		int maxAsteroids = 5;
		int asteroidsCounter = 0;

		for (auto& pair : factory->gameObjects)
		{
			// check if object is asteriod
			if (pair.second.obj_type == TYPE_ASTEROID)
			{
				asteroidsCounter++;

				auto& object = pair.second;

				//players need to know the gameObjectID of each gameObject
				//push gameObjectID
				Insert_Number_MessageList(messageList, pair.first);

				//players need to know the details of each gameObjects
				InsertGameObject_MessageList(messageList, object);

				//if number of asteroids have reached 5 in the packet
				if (asteroidsCounter == maxAsteroids)
				{
					//players need to know the number of gameObjects in each message
					//push number of gameObjects
					FrontInsert_Number_MessageList(messageList, asteroidsCounter);

					//send broadcast messsage
					_InsertNotification(GameCommands::SyncrhoniseAsteroids, messageList, -1);

					//restart the asteroids counter
					asteroidsCounter = 0;
					//restart messageList
					messageList.clear();
				}

			}
		}

		//last packet to send
		if (asteroidsCounter)
		{
			//players need to know the number of gameObjects in each message
			//push number of gameObjects
			FrontInsert_Number_MessageList(messageList, asteroidsCounter);

			//send broadcast messsage
			_InsertNotification(GameCommands::SyncrhoniseAsteroids, messageList, -1);

			//restart the asteroids counter
			asteroidsCounter = 0;
			//restart messageList
			messageList.clear();
		}
	}


	// send a syn command, of player position, rotation, velocity
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

				if (factory->_playerID == object.playerIndex)
					factory->_playerObjectID = gameObjectID;
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
		else if (command == GameCommands::Synchronise_Player)
		{
			GameObject object;
			ExtractGameObject_MessageList(0, messageList, object);

			GameObject& player_toSyn = factory->getPlayer(object.playerIndex);
			
			PRINTOUT("Before Synchronise : \n");
			PRINTOUT("player position x : ", player_toSyn.transform.position.x , "\n");
			PRINTOUT("player position y : ", player_toSyn.transform.position.y , "\n");
			

			player_toSyn = object;

			PRINTOUT("\n After Synchronise : \n");
			PRINTOUT("player position x : ", player_toSyn.transform.position.x, "\n");
			PRINTOUT("player position y : ", player_toSyn.transform.position.y, "\n");

		}
		else if (command == GameCommands::Shoot)
		{
			Vector2 position;
			float rotation;
			Vector2 velocity;
			int playerindex;

			Extract_Number_MessageList(0, messageList, position.x);
			Extract_Number_MessageList(1, messageList, position.y);
			Extract_Number_MessageList(2, messageList, rotation);
			Extract_Number_MessageList(3, messageList, velocity.x);
			Extract_Number_MessageList(4, messageList, velocity.y);
			Extract_Number_MessageList(5, messageList, playerindex);
			factory->CreateBullet(position, rotation, velocity, playerindex);
		}
		else if (command == GameCommands::DestroyObject)
		{
			int ID;
			Extract_Number_MessageList(0, messageList, ID);
			factory->DeleteGameObjectID(ID);
		}
		else if (command == GameCommands::UpdateScore)
		{
			int playerindex;
			Extract_Number_MessageList(0, messageList, playerindex);

			if (_playerID == playerindex)
			{
				_score += asteriod_reward;
				std::cout << "New Score : " << _score << std::endl;
			}
		}
	}
	EventsList.clear();

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

bool LogicSystem::CollisionIntersection(const AABBCollider& aabb1, const AABBCollider& aabb2)
{
	if (aabb1.max.x < aabb2.min.x || aabb1.min.x > aabb2.max.x)
		return false;

	if (aabb1.max.y < aabb2.min.y || aabb1.min.y > aabb2.max.y)
		return false;

	return true;
}

void LogicSystem::CheckCollision(Factory* factory)
{
	for (auto& pair : factory->gameObjects)
	{
		if (pair.second.obj_type == TYPE_ASTEROID)
		{
			for (auto& pair2 : factory->gameObjects)
			{
				if (pair2.second.obj_type == TYPE_BULLET)
				{
					bool isCollided = CollisionIntersection(pair.second.aabb, pair2.second.aabb);
					if (isCollided)
					{
						factory->DeleteGameObjectID(pair.first);
						factory->DeleteGameObjectID(pair2.first);

						MessageList messageList;

						Insert_Number_MessageList(messageList, pair.first);
						_InsertNotification(GameCommands::DestroyObject, messageList, -1);


						messageList.clear();

						Insert_Number_MessageList(messageList, pair2.first);
						_InsertNotification(GameCommands::DestroyObject, messageList, -1);

						// update score, pair2.second is the bullet
						if (pair2.second.playerIndex == 0)
						{
							_score += asteriod_reward; 
							std::cout << "New Score : " << _score << std::endl;
						}
						else
						{
							MessageList messageList;
							Insert_Number_MessageList(messageList, pair2.second.playerIndex);
							_InsertNotification(GameCommands::UpdateScore, messageList, -1);
						}
					}
				}

				// collision of asteriod and player

				/*else if (pair2.second.obj_type == TYPE_PLAYER)
				{
					bool isCollided = CollisionIntersection(pair.second.aabb, pair2.second.aabb);
					if (isCollided)
					{
						factory->DeleteGameObjectID(pair2.first);
					}
				}*/

			}
		}

	}
}
