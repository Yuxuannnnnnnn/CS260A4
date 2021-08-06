#pragma once

#include <functional>
#include <thread>
#include "MessageFormat.h"
#include "clientUDPSocket.h"

#include <mutex>





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

	//function to let the logicSystem know if this player is the host
	typedef std::function<void(
		bool isHost,
		std::vector<int>& playerIndexList,
		int playerID)> 
		IndicateHostFunction;
	IndicateHostFunction _IndicateHost;


	//own client udp socket
	clientUDPSocket clientUDPsock;

	int PlayerIndex = 0;
	std::mutex PlayerIndexMutex;


	//stores indices to get ClientSockIndex
	typedef int playerInd;
	typedef int ClientSockInd;
	std::unordered_map<playerInd, ClientSockInd> PlayerIndex_ClientSockIndex;
	typedef std::vector<int> playerIndexList;
	playerIndexList playerIndices;
	std::mutex playerIndicesMutex;



	//List of notifications for the networkSystem to send during update()
	std::vector<Message> notificationList;


public:

	void Init(const InsertEventFunction& InsertEvent, 
		const IndicateHostFunction& IndicateHost)
	{
		//get function to insert events to logicSystem
		_InsertEvent = InsertEvent;
		//get function to let the logicSystem know if this player is the host
		_IndicateHost = IndicateHost;


		//Start up winsock
		clientUDPsock.Startup();
	}

//------------------------------------------------------------------------------------


	void Wait_ToConnectAllClients(Hostname_Port_List& list)
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
				clientUDPsock.Get_Store_ClientAddress(
				list[i].second, 
				list[i].first);

		}
		


//-------------------Wait for all Clients to connect-------------------------

		typedef int Index;
		std::unordered_map<Index, sockaddr>& clientAddressIndices = 
			clientUDPsock.GetIndexAddrList();

		//wait for clients to respond
		std::vector<std::thread> clientThreads;

		//run each thread for each receiving address
		for (auto& pair: clientAddressIndices)
		{
			//get the clientAddressIndex
			int clientIndex = pair.first;

			//start a thread to receive events from each client
			clientThreads.push_back(std::thread{
				&NetworkSystem::WaitClientConnect,
				std::ref(*this), clientIndex });
		}

	//send a packet to all players that this client has just joinedGame
	//all players who received this message should know
	//this player's sockaddr
		Message JoinedGameMessage = CreateClientMessage(
			GameCommands::JoinGame);
		clientUDPsock.BroadcastMessage(JoinedGameMessage);


		//wait for all clients to join
		for (size_t i = 0; i < clientThreads.size(); i++)
		{
			clientThreads[i].join();
		}
		clientThreads.clear();

		//One client will be the host
		if (PlayerIndex == 0)
		{
			_IndicateHost(true, playerIndices, PlayerIndex);
		}

//---------------All clients have connected from this point-------------------------

		//broadcast to everyone, my player Index
		Message SendIndexMessage = CreateClientMessage(
			GameCommands::SendIndex, { {(char*)&PlayerIndex, sizeof(PlayerIndex)} });
		clientUDPsock.BroadcastMessage(SendIndexMessage);


		//run each thread for each receiving address
		for (auto& pair : clientAddressIndices)
		{
			//get the clientAddressIndex
			int clientIndex = pair.first;

			//start a thread to receive events from each client
			clientThreads.push_back(std::thread{
				&NetworkSystem::WaitClientAnnounceIndex,
				std::ref(*this), clientIndex });
		}

		//wait for all clients to join
		for (size_t i = 0; i < clientThreads.size(); i++)
		{
			clientThreads[i].join();
		}

	}


	void WaitClientAnnounceIndex(int clientAddressIndex)
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

			//wait for clients to respond their playerIndex
			if (Command == GameCommands::SendIndex)
			{
				//extract playerID
				int playerID = -1;
				memcpy(&playerID, message[0].message, message[0].size_);
				//store playerID with the clientAddressID
				PlayerIndex_ClientSockIndex[playerID] = clientAddressIndex;

				std::lock_guard<std::mutex> playerIndiceslock(playerIndicesMutex);
				playerIndices.push_back(playerID);
				break;
			}
		}
	}

	//helper function for Wait_ToConnectAllClients()
	//to run per thread for each receiving client
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
				//count the number of players already in the game
				//so that I can assign a playerID to myself at the end
				std::lock_guard<std::mutex> IndexCounterLock{ PlayerIndexMutex };
				PlayerIndex ++;

				char buffer[100] = "\0";

				std::cout << "Client" 
					<< inet_ntop(AF_INET, &clientUDPsock.Index_Addresses[clientAddressIndex],
						buffer, 100)
					<< "is in game." << std::endl;

				break;

			}
			//if clients never respond, then wait for them to send a joinedGame notification
			//Respond to the client an InGame notification
			else if (Command == GameCommands::JoinGame)
			{
				Message InGameMessage = CreateClientMessage(
					GameCommands::InGame);
				//clientUDPsock.SendClientMessage(clientAddressIndex, InGameMessage);
				clientUDPsock.BroadcastMessage(InGameMessage, clientAddressIndex);
				char buffer[100] = "\0";

				std::cout << "Client"
					<< inet_ntop(AF_INET, &clientUDPsock.Index_Addresses[clientAddressIndex],
						buffer, 100)
					<< "has joined game." << std::endl;



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
	void BroadcastEventsToClients()
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
		for (auto& pair: PlayerIndex_ClientSockIndex)
		{
			//get the clientAddressIndex
			int clientIndex = pair.second;

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
		int playerIndex = -1;

		for (auto& pair : PlayerIndex_ClientSockIndex)
			if (pair.second == clientAddressIndex)
				playerIndex = pair.first;

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
			_InsertEvent(playerIndex, //indicates player ID
				{ Command, message });
		}
	}

};

