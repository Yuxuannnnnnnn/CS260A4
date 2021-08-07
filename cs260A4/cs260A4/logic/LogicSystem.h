#pragma once

#include "../input/InputSystem.h"
#include "../input/KeyEnum.h"
#include <vector>
#include <mutex>
#include <functional>
#include <string>
#include "DRData.h"
#include "../core/GameObject.h"

#include <typeinfo> //For typeid() function

//for gameObjects creation 
//store factory pointer in LogicSystem
#include "../core/Factory.h"

//for event
#include "../network/MessageFormat.h"
#include "../network/GameCommands.h"


//for PRINTOUT function
#include "../Tools/EngineSettings.h"

#define PI 3.1415926f


class LogicSystem
{
	typedef std::vector<Message> MessageList;
	typedef std::pair<GameCommands, MessageList> Event;
	typedef int clientsAddressIndex;
	typedef std::pair<clientsAddressIndex, Event> clientIndex_Event_Pair ;
	//list of playerIndex & Event Pair
	//the logic system has to clear this eventsList every loop
	std::vector<clientIndex_Event_Pair> EventsList;
	std::mutex EventsList_Mutex;


	//store the vector index of the events in the EventsList to be deleted
	//Store the Events that should be deleted after pulled
	//after traversing the eventList
	std::vector<int> EventsDeletionList;


	//for logicSystem update to use 
	//callback function to insert a notification to the 
	//notification list in the networking system
	//Function to Send notifications to other clients
	typedef std::function<void(const GameCommands& command,
												//clientAddressIndex = -1 refers to send to everyone
		const std::vector<Message> messageList, int clientAddressIndex)>
		InsertNotificationFunction;
	InsertNotificationFunction _InsertNotification;


	//indicates the number of players in the Game
	//If the commandline has 4 addresses, the NumOfPlayersRequired will be 4
	int _NumOfPlayersRequired{ 0 };


	//stores the map to get the playerIndex from the clientsAddressIndex
	typedef int playerID;
	std::unordered_map<clientsAddressIndex, playerID> clientAddrID_PlayerID_List;


	//boolean for whether this player is a host
	//The client that is the host
	//will define the settings of the asteroids throughout the game & 
	//and the initial settings of all the player ships
	bool _isHost{ false };

	//Own player ID
	playerID _playerID{0};

	//store factory - for game objects creation
	Factory* gameFactory;


public:

//--------------------Initialise the LogicSystem------------------------------------

	void Init(Factory* factory,
		const InsertNotificationFunction& InsertNotification, 
		int NumOfPlayersRequired)
	{
		//get function to insert notification to networkSystem
		_InsertNotification = InsertNotification;

		//If the commandline has 4 addresses, the NumOfPlayersRequired will be 4
		_NumOfPlayersRequired = NumOfPlayersRequired;

		//store factory - for game objects creation
		gameFactory = factory;
	}

//--------------------Waiting for players to Join game------------------------------------

	void Wait_ForAllPlayers()
	{
	//------------Wait for all Clients to connect----------------------

		//send a packet to all players that this client has just joinedGame
		//all players who received this message should know
		//this player's sockaddr
		_InsertNotification(GameCommands::JoinGame, {}, -1);

		int count_players_in_game = 1; //count this client

		//while the number of players in the game has not reached the requirement
		while (count_players_in_game < _NumOfPlayersRequired)
		{
			//make sure the network System is not insert events to the list
			//while logicSystem is accessing the eventsList
			std::lock_guard<std::mutex> EventsListLock{ EventsList_Mutex };
			for(int i = 0; i < EventsList.size(); i++)
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

					//count the number of players already in the game
					//so that I can assign a playerID to myself at the end
					++_playerID;
				

					PRINTOUT("ClientAddressIndex: "
						, clientAddrIndex
						, " is in game.");
					PRINTOUT("Player "
						, _playerID -1
						, " is in game.");

					//increment number of players in the game
					count_players_in_game++;
				}
				//if clients never respond, then wait for them to send a joinedGame notification
				else if (command == GameCommands::JoinGame)
				{
					//add this event to be deleted
					EventsDeletionList.push_back(i);

					//Respond to the client an InGame notification
					_InsertNotification(GameCommands::InGame, {}, clientAddrIndex);

					PRINTOUT("ClientAddressIndex: "
						, clientAddrIndex,
						" has joined game.");
					PRINTOUT("Player "
						, count_players_in_game
						, " has joined game.");

					//increment number of players in the game
					count_players_in_game++;
				}
			}


			clearEventsToBeDeleted();
		}

		//if the playerID of this client is 0
		//then this client will be the host
		//that define the settings of the asteroids throughout the game& 
		//and the initial settings of all the player ships
		if (_playerID == 0)
		{
			HostPlayer(true);
		}

	//---------------All clients have connected from this point----------------------

		//broadcast to everyone, my player Index
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
			for (auto& events : EventsList)
			{
				clientsAddressIndex clientAddrIndex = events.first;
				Event event = events.second;
				GameCommands command = event.first;
				MessageList messageList = event.second;

				//wait for clients to respond their clientsAddressIndex
				if (command == GameCommands::SendIndex)
				{
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

	//--------All clients have made known their playerID to every client at this point----------


	}


	void clearEventsToBeDeleted()
	{
		for (auto& deleteEvent : EventsDeletionList)
		{
			EventsList.erase(EventsList.begin() + deleteEvent);
		}

		EventsDeletionList.clear();
	}

	//Configure if this client is a HostPlayer
	void HostPlayer(bool isHost)
	{
		_isHost = isHost;
	}


//-------------------------Host Initialisation of Game------------------------------------


	void HostInitGame()
	{
		//if this client is the host, then is the host responsibility
		//to define the settings of the asteroids throughout the game & 
		//and the initial settings of all the player ships
		if (_isHost)
		{
			MessageList messageList;

			auto ShipList = 
				gameFactory->Create_playerShips_DataInitialisation(
				_NumOfPlayersRequired,
				_playerID);

			auto AsteroidsList = 
				gameFactory->Create_Asteroids_DataInitialisation();

			//players need to know the number of gameObjects in each message
			//push number of gameObjects
			Insert_Number_MessageList(messageList, ShipList.size());

			for (auto& object : ShipList)
			{
				//players need to know the gameObjectID of each gameObject
				//push gameObjectID
				Insert_Number_MessageList(messageList, object.second);

				//players need to know the details of each gameObjects
				GameObject& obj = gameFactory->getGameObject(object.second);
				InsertGameObject_MessageList(messageList, obj);
			}
			_InsertNotification(GameCommands::SynchronisePlayer, messageList, -1);


			messageList.clear();

			//players need to know the number of gameObjects in each message
			//push number of gameObjects
			Insert_Number_MessageList(messageList, AsteroidsList.size());

			for (auto& object : AsteroidsList)
			{
				//players need to know the gameObjectID of each gameObject
				//push gameObjectID
				Insert_Number_MessageList(messageList, object);

				//players need to know the details of each gameObjects
				GameObject& obj = gameFactory->getGameObject(object);
				InsertGameObject_MessageList(messageList, obj);
			}

			_InsertNotification(GameCommands::SyncrhoniseAsteroids, messageList, -1);


		}
	}


//Insert to MessageList---------------------

	void InsertGameObject_MessageList(MessageList& messageList, const GameObject& object)
	{
		Insert_Vec2_MessageList(messageList, object.transform.position);
		Insert_Vec2_MessageList(messageList, object.transform.scale);
		Insert_Number_MessageList(messageList, object.transform.rotation);

		Insert_Number_MessageList(messageList, object.mesh);
		Insert_Vec3_MessageList(messageList, object.color);

		Insert_Number_MessageList(messageList, object.obj_type);
		Insert_Number_MessageList(messageList, object.playerIndex);
	}


	//function to convert a Vec3 to a string and then to a Message
	//Insert the Mesage to the messageList
	void Insert_Vec3_MessageList(MessageList& messageList, const Vector3& vec)
	{
		Insert_Number_MessageList(messageList, vec.x);
		Insert_Number_MessageList(messageList, vec.y);
		Insert_Number_MessageList(messageList, vec.z);
	}

	//function to convert a Vec2 to a string and then to a Message
	//Insert the Mesage to the messageList
	void Insert_Vec2_MessageList(MessageList& messageList, const Vector2& vec)
	{
		Insert_Number_MessageList(messageList, vec.x);
		Insert_Number_MessageList(messageList, vec.y);
	}


	//function to convert an integer/float to a string and then to a Message
	//Insert the Mesage to the messageList
	template<typename type>
	void Insert_Number_MessageList(MessageList& messageList, const type& number)
	{
		messageList.push_back({ std::to_string(number).c_str(),
		std::to_string(number).size() + sizeof("\0") });
	}


//Extract From MessageList---------------------

	void ExtractGameObject_MessageList(int index, MessageList& messageList, GameObject& object)
	{
		Extract_Vec2_MessageList(index, messageList, object.transform.position);
		Extract_Vec2_MessageList(index, messageList, object.transform.scale);
		Extract_Number_MessageList(index, messageList, object.transform.rotation);

		Extract_Number_MessageList(index, messageList, object.mesh);
		Extract_Vec3_MessageList(index, messageList, object.color);

		Extract_Number_MessageList(index, messageList, object.obj_type);
		Extract_Number_MessageList(index, messageList, object.playerIndex);
	}


	//function to extract an vec3 from the messageList
	void Extract_Vec3_MessageList(int index, MessageList& messageList, Vector3& vec)
	{
		Extract_Number_MessageList(index, messageList, vec.x);
		Extract_Number_MessageList(index, messageList, vec.y);
		Extract_Number_MessageList(index, messageList, vec.z);
	}

	//function to extract an vec2 from the messageList
	void Extract_Vec2_MessageList(int index, MessageList& messageList, Vector2& vec)
	{
		Extract_Number_MessageList(index, messageList, vec.x);
		Extract_Number_MessageList(index, messageList, vec.y);
	}


	//function to extract an integer or float from the messageList
	template<typename type>
	void Extract_Number_MessageList(int index, MessageList& messageList, type& number)
	{
		Message mess = messageList[index];
		char buffer[100] = "\0";
		memcpy(&buffer, mess.message, mess.size_);

		std::string NumberString = buffer;

		if (typeid(type) == typeid(int))
		{
			number = (type)stoi(NumberString);
		}
		else if (typeid(type) == typeid(float))
		{
			number = (type)stof(NumberString);
		}
	}


	


//--------------------------------In the GameLoop------------------------------------


	//when looping through the eventslist in the update
	//should lock the mutex before entering the loop
	void Update(const InputSystem& inputsystem, float dt, float gametime, Factory*);

//----------------------Functions For NetworkSystem to use------------------------------------

	//a function for the NetworkSystem to use
	//when the networkSystem received a packet
	//from the other players
	//inserts an Event for the logicSystem to process
	void InsertEvent(
		int clientAddressIndex, 
		const Event& _event)
	{
		std::lock_guard<std::mutex> EventsListLock{ EventsList_Mutex };
		//stores the event in the EventsList
		EventsList.push_back({ clientAddressIndex,_event });
	}


private:

	int _loopCounter = 0;
	int _synCount = 10;

	void PullEvent(float currgametime, Factory*);
	void SynchronisePosition();
	void PerformDR(GameObject& ship, const DRData& drdata, float currgametime);

	float Wrap(float x, float x0, float x1);
	float acceleration_speed = 40.0f;
	float rotation_speed = 6.3f;
};