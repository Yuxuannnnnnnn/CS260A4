#pragma once

#include <functional>
#include <thread>
#include "MessageFormat.h"
#include "clientUDPSocket.h"




//the network system runs multiple threads
//to receive packets from socket 
//from each address
//stores the packets in
class NetworkSystem
{
	typedef std::string HostnameString;
	typedef std::string PortString;
	typedef  std::vector<std::pair<HostnameString, PortString>> Hostname_Port_List;


	typedef std::vector<Message> message;
	typedef std::pair<GameCommands, message> Event;
	typedef std::function<void(int, const Event& _event)>
		InsertEventFunction;
	InsertEventFunction _InsertEvent;

	clientUDPSocket clientUDPsock;

	//stores indices to get the sockaddr from clientUDPSocket
	std::vector<int> clientAddressIndices;

	//List of notifications for the networkSystem to send during update()
	std::vector<Message> notificationList;

public:

	void Init(const InsertEventFunction& InsertEvent)
	{
		//get function to insert events to logicSystem
		_InsertEvent = InsertEvent;
		//Start up winsock
		clientUDPsock.Startup();
	}

//------------------------------------------------------------------------------------

	//returns list of clients' indices
	std::vector<int> Wait_ToConnectAllClients(Hostname_Port_List& list)
	{
		//Bind own UDP socket
		clientUDPsock.Prep_UDPClientSocket(
			//get own Port number
			list[0].second
		);

		//store all the other clients addresses
		for (size_t i = 1; i < list.size(); i++)
		{
			//store the address index returned from the function
			clientAddressIndices.push_back(
				clientUDPsock.Get_Store_ClientAddress(
				list[i].second, 
				list[i].first));

		}


		//send a packet to all players that this client has just joinedGame
		//all players who received this message should know
		//this player's sockaddr
		Message JoinedGameMessage = CreateClientMessage(
			GameCommands::JoinGame);
		clientUDPsock.BroadcastMessage(JoinedGameMessage);

		
		//wait for clients to respond
		std::vector<std::thread> clientThreads;

		//run each thread for each receiving address
		for (size_t i = 0; i < clientAddressIndices.size(); i++)
		{
			//get the clientAddressIndex
			int clientIndex = clientAddressIndices[i];

			//start a thread to receive events from each client
			clientThreads.push_back(std::thread{
				&NetworkSystem::WaitClientConnect,
				std::ref(*this), clientIndex });
		}

		//wait for all clients to join
		for (size_t i = 0; i < clientThreads.size(); i++)
		{
			clientThreads[i].join();
		}


		//All clients have connected from this point

		return clientAddressIndices;
	}

	void WaitClientConnect(int clientAddressIndex)
	{
		//continuously receiveMessage from this client
		while (true)
		{
			GameCommands Command = GameCommands::TotalCommands;
			std::vector<Message> message;

			int success = 1;

			//receive client Command and Messages
			success = clientUDPsock.ReceiveMessage(
				clientAddressIndex,
				Command,
				message);

			//wait for clients to respond that they are already inGame
			if (Command == GameCommands::InGame)
			{
				break;
			}
			//if clients never respond, then wait for them to send a joinedGame notification
			//Respond to the client an InGame notification
			else if (Command == GameCommands::JoinGame)
			{
				Message InGameMessage = CreateClientMessage(
					GameCommands::InGame);
				clientUDPsock.SendClientMessage(clientAddressIndex, InGameMessage);
				break;
			}
		}
	}

//------------------------------------------------------------------------------------


	//callback function for all the other systems to use
	//when they have a message to notify all other clients
	void InsertNotification(const GameCommands& command,
		const std::vector<Message> messageList = std::vector<Message>{})
	{
		notificationList.push_back(CreateClientMessage(command, messageList));
	}

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



//------------------------------------------------------------------------------------
	
	//Inside GameLoop
	//send notifications to all other clients
	void Update()
	{
		//loop through the notifiications List
		//then broadcast the messages to all clients

		for (auto& notif: notificationList)
		{
			clientUDPsock.BroadcastMessage(notif);
		}
	}



//--------------------------------------------------------------------------------



	void ReceiveEvents()
	{
		std::vector<std::thread> clientThreads;

		//run each thread for each receiving address
		for (size_t i = 0; i < clientAddressIndices.size(); i++)
		{
			//get the clientAddressIndex
			int clientIndex = clientAddressIndices[i];

			//start a thread to receive events from each client
			clientThreads.push_back(std::thread{
				&NetworkSystem::ReceiveEventFromClient,
				std::ref(*this), clientIndex });
		}

		//wait for all clients to join
		for (size_t i = 0; i < clientThreads.size(); i++)
		{
			clientThreads[i].join();
		}
	}
	
	void ReceiveEventFromClient(int clientAddressIndex)
	{
		//continuously receiveMessage from this client
		while (true)
		{
			GameCommands Command;
			std::vector<Message> message;

			int success = 1;

			//receive client Command and Messages
			success = clientUDPsock.ReceiveMessage(
				clientAddressIndex,
				Command,
				message);

			if (success == 0 || success == 2)
			{
				//Command = client disconnected
			}

			//insert the message to the logicSystem eventlist
			_InsertEvent(clientAddressIndex, //indicates player ID
				{ Command, message });
		}
	}

};

