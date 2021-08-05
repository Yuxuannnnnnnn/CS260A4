#pragma once

#include "../input/InputSystem.h"
#include "../input/KeyEnum.h"
#include <vector>
#include <mutex>
#include <functional>

//for event
#include "../network/MessageFormat.h"
#include "../network/GameCommands.h"

#define PI 3.1415926f

class LogicSystem
{
	typedef std::vector<Message> message;
	typedef std::pair<GameCommands, message> Event;
	std::vector<Event> EventsList;
	std::mutex EventsList_Mutex;


	typedef std::function<void(const GameCommands& command,
		const std::vector<Message> messageList)>
		InsertNotificationFunction;

	//callback function to insert a notification to the 
	//notification list in the networking system
	InsertNotificationFunction _InsertNotification;

public:

	void Init(const InsertNotificationFunction& InsertNotification)
	{
		//get function to insert notification to networkSystem
		_InsertNotification = InsertNotification;
	}

	//when looping through the eventslist in the update
	//should lock the mutex before entering the loop
	void Update(const InputSystem& inputsystem, float dt);


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
		EventsList.push_back(_event);
	}

private:


	float Wrap(float x, float x0, float x1);
	float acceleration_speed = 40.0f;
	float rotation_speed = 6.3f;
};