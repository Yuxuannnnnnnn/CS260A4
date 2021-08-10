#pragma once



#ifndef NETWORKSYSTEM
#define NETWORKSYSTEM


#include <functional>
#include <thread>
#include "MessageFormat.h"
#include "GameCommands.h"
#include "clientUDPSocket.h"

#include <mutex>
#include "../Tools/Timer.h"
#include "../Tools/EngineSettings.h"
#include <set>
#include <string>
#include <algorithm>

class LogicSystem;

//the network system runs multiple threads
//to receive packets from socket 
//from each address
//stores the packets in
class NetworkSystem
{
	typedef std::string HostnameString;
	typedef std::string PortString;
	typedef  std::vector<std::pair<HostnameString, PortString>> Hostname_Port_List;


	//function to insert an event to the logicSystem Eventlist
	typedef std::vector<Message> message;
	typedef std::pair<GameCommands, message> Event;
	typedef std::function<void(int, const Event& _event)>
		InsertEventFunction;
	InsertEventFunction _InsertEvent;


	//own client udp socket
	clientUDPSocket clientUDPsock;


	//List of notifications for the networkSystem to send during update()
	std::vector<Message> notificationList;


public:

//--------------------Initialise the NetworkSystem------------------------------------


	//Initialise the NetworkSystem
	void Init(Hostname_Port_List& list,
		const InsertEventFunction& InsertEvent);


//--------------------for all the other systems to use------------------------------------


	bool checkUser_HostnamePort(
		const std::string& hostName,
		const std::string& Port,
		int& clientAddrIndex);

	//callback function for all the other systems to use
	//when they have a message to notify all other clients
	void InsertNotification(const GameCommands& command,
		const std::vector<Message> messageList = std::vector<Message>{},
		int clientAddressIndex = -1);

	//helper function to create a message segment for socket to send
	Message CreateClientMessage(
		const GameCommands& command,
		const std::vector<Message> messageList = std::vector<Message>{});


//---------------------Runs Seperate from the MainThread----------------------------

	void ReceiveEventsFromClient(LogicSystem* logicSystem);

};



#endif