#pragma once

#include <functional>
#include <thread>
#include "MessageFormat.h"
#include "clientUDPSocket.h"

#include <mutex>
#include "../Tools/Timer.h"
#include "../Tools/EngineSettings.h"
#include <set>
#include <string>
#include <algorithm>




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
		const InsertEventFunction& InsertEvent)
	{
		//get function to insert events to logicSystem
		_InsertEvent = InsertEvent;

		//Start up winsock
		clientUDPsock.Startup();

		//Bind own UDP socket
		clientUDPsock.Prep_UDPClientSocket(
			//get own Port number
			list[0].second
		);

		//store all the other clients addresses
		for (size_t i = 1; i < list.size(); i++)
		{
			//store the address index returned from the function
			clientUDPsock.Get_Store_ClientAddress(
				list[i].second,
				list[i].first);

		}
	}


//--------------------for all the other systems to use------------------------------------


	//callback function for all the other systems to use
	//when they have a message to notify all other clients
	void InsertNotification(const GameCommands& command,
		const std::vector<Message> messageList = std::vector<Message>{}, 
		int clientAddressIndex = -1)
	{
		//if no clientAddressIndex is indicated in the paramters
		//then broadcast the message to everyone

		Message mess = CreateClientMessage(command, messageList);

		if(clientAddressIndex == -1)
			clientUDPsock.BroadcastMessage(mess);
		else //send to the specific client
		{
			clientUDPsock.SendClientMessage(clientAddressIndex,
				mess);
		}

	}

	//helper function to create a message segment for socket to send
	Message CreateClientMessage(
		const GameCommands& command,
		const std::vector<Message> messageList = std::vector<Message>{})
	{
		char Command = (char)command; //get command 

		int PayloadLength = 1/*null char*/; //get Payload Length

		//calculate the PayloadLength
		for (auto& i : messageList)
		{
			PayloadLength += sizeof(short);
			PayloadLength += (int)i.size_;
		}

		size_t MessageBufferSize =
			sizeof(Command)
			+ sizeof(PayloadLength)
			+ PayloadLength;

		char MessageBuffer[1500] = "";

		//Add nullchar to end of MessageBuffer
		MessageBuffer[MessageBufferSize - 1] = '\0';

		size_t offset_size = 0;

		//copy Command and PayloadLength into MessageBuffer
		memcpy_s(MessageBuffer, MessageBufferSize, &Command, sizeof(Command));
		offset_size += sizeof(Command);

		//convert host byte to network byte
		PayloadLength = htonl(PayloadLength);
		memcpy_s(MessageBuffer + offset_size,
			MessageBufferSize - offset_size,
			&PayloadLength,
			sizeof(PayloadLength));
		offset_size += sizeof(PayloadLength);

		//copy messages into MessageBuffer
		for (auto& i : messageList)
		{
			//get size of message
			short size = (short)(i.size_);
			//convert host byte to network byte
			size = htons(size);

			//Copy size of message into MessageBuffer
			memcpy_s(MessageBuffer + offset_size,
				MessageBufferSize - offset_size,
				&size,
				sizeof(short));
			offset_size += sizeof(short);

			//Copy message into MessageBuffer
			memcpy_s(MessageBuffer + offset_size,
				MessageBufferSize - offset_size,
				i.message,
				i.size_);
			offset_size += i.size_;
		}

		//std::string message = MessageBuffer;
		//delete MessageBuffer;

		return Message{ MessageBuffer, MessageBufferSize };

	}


//---------------------Runs Seperate from the MainThread----------------------------

	void ReceiveEventsFromClient()
	{
		//continuously receiveMessage from this client
		while (true)
		{
			//find out which client sent the message
			int clientAddressIndex = -1;

			GameCommands Command;
			std::vector<Message> message;

			int success = 1;

			//receive client Command and Messages
			success = clientUDPsock.ReceiveMessage(
				clientAddressIndex,
				Command,
				message);


			//0 - socketerror
			//2 - 0 bytes received
			if (success == 0 || success == 2 || clientAddressIndex == -1)
			{
				//Command = client disconnected
			}
			else
			{
				//insert the message to the logicSystem eventlist
				_InsertEvent(clientAddressIndex, //indicates clientsAddressIndex
												 //who the message is coming from
					{ Command, message });
			}
		}
	}

};

