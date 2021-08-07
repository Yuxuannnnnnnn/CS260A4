#pragma once

#include "../input/InputSystem.h"
#include "../input/KeyEnum.h"
#include <vector>
#include <mutex>
#include <functional>
#include "DRData.h"
#include "../core/GameObject.h"

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
			auto ObjectList = 
				gameFactory->Create_playerShips_DataInitialisation(
				_NumOfPlayersRequired,
				_playerID);

			//players need to know the gameObjectID of each gameObject
			//players need to know the playerID of each ship
			//players need to know the details of each gameObjects

			for (auto& object : ObjectList)
			{

			}


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