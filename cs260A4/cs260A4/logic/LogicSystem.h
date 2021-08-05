#pragma once

#include "../input/InputSystem.h"
#include "../input/KeyEnum.h"
#include <vector>
#include <mutex>
#include <functional>
#include "DRData.h"
#include "../core/GameObject.h"

//for event
#include "../network/MessageFormat.h"
#include "../network/GameCommands.h"

#define PI 3.1415926f

class LogicSystem
{
	typedef std::vector<Message> message;
	typedef std::pair<GameCommands, message> Event;

	typedef int playerIndex;
	typedef std::pair<playerIndex, Event> playerIndex_Event_Pair ;
	//list of playerIndex & Event Pair
	//the logic system has to clear this eventsList every loop
	std::vector<playerIndex_Event_Pair> EventsList;
	std::mutex EventsList_Mutex;


	typedef std::function<void(const GameCommands& command,
		const std::vector<Message> messageList)>
		InsertNotificationFunction;

	//for logic update to use 
	//callback function to insert a notification to the 
	//notification list in the networking system
	InsertNotificationFunction _InsertNotification;



	//boolean for whether this player is a host
	bool _isHost{ false };
	//list of playerindex		
	std::vector<playerIndex> _playerIndexList;
	//Own player ID
	int _playerID;


public:

	//callback function for network system use 
	//to configure if this client is a HostPlayer
	void HostPlayer(bool isHost, 
		std::vector<int>& playerIndexList, 
		int playerID)
	{
		_isHost = isHost;
		//used for creation of ship
		_playerIndexList = playerIndexList;

		_playerID = playerID;
	}


	void Init(const InsertNotificationFunction& InsertNotification)
	{
		//get function to insert notification to networkSystem
		_InsertNotification = InsertNotification;
	}

	//when looping through the eventslist in the update
	//should lock the mutex before entering the loop
	void Update(const InputSystem& inputsystem, float dt, float gametime);


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

	void PullEvent(float currgametime);
	void SynchronisePosition();
	void PerformDR(GameObject* ship, float currgametime, float drtime, Vector2 accleration);

	float Wrap(float x, float x0, float x1);
	float acceleration_speed = 40.0f;
	float rotation_speed = 6.3f;
};