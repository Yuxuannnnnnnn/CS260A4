#pragma once

/*!*****************************************************************************
\file ClientTCPSocket.cpp
\author Luo Yu Xuan
\par DP email: yuxuan.luo\@digipen.edu
\par Course: CS260
\par Section: A
\par Programming Assignment 3
\date 07-9-2021
\brief
This file contains the wrapper for Winsock Application and
the Client Socket.
*******************************************************************************/

#ifndef CLIENT_UDP_SOCKET
#define CLIENT_UDP_SOCKET


//Tell the Visual Studio linker to include the following library in linking.
 //Alternatively, we could add this file to the linker command-line parameters, 
 // but including it in the source code simplifies the configuration.
#pragma comment (lib, "ws2_32.lib")

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef WINDOWS
#define WINDOWS
#include "ws2tcpip.h"		// getaddrinfo()
#include "Windows.h"		// Entire Win32 API...
#endif



#include <iostream>			// cout, cerr
#include <string>			// string
#include <unordered_map>
#include <array>

#include "MessageFormat.h"
#include "GameCommands.h"


/*******************************************************************************
 * A simple UDP/IP client application
 ******************************************************************************/




class clientUDPSocket
{
public:

    //Socket
    SOCKET clientUDPSock_{}; 

    std::string clientPortString_{};
    std::string clientUDP_HostNameString_{};


    //Stores the list of addresses needed
    // 
    //To, send data to               
            // - max is 1 address at a time
    //and also to, receive data from 
            //- can have any amount of address at a time
    typedef int Index;  //just an integer to keep track 
    Index AddressIndex{ 0 }; //increments when a new address is added
    std::unordered_map<Index, sockaddr> Index_Addresses;


    //A value  indicting a rate of packet loss simulated 
        //during receiving of data over UDP.
    float RateOfPacketLoss_{ 0.0f };

//-------------------------------------------------------------------------

    //default constructor
    clientUDPSocket()
    {
    }


    //custom constructor
    clientUDPSocket(const float& RateOfPacketLoss)
        :RateOfPacketLoss_{ RateOfPacketLoss}
    {}


    //Copy Constructor
    clientUDPSocket(const clientUDPSocket& clientSocket);

    //Starts up the Winsock application
    //returns errorCode of WSAStartup()
    int Startup();


    //Create UDP socket and Bind it to own network interface controller.
    int Prep_UDPClientSocket(
        // own Port number - from config file
        const std::string& clientPortString
    );


    //get and store the clientAddress to send/recv data from
        //if fail to get the clientAddress, then return -1
        //if succeed to get the clientAddress, then return the client index
    int Get_Store_ClientAddress(
        //Port number & IPaddress/HostName of client 
        //to send/recv data from
        const std::string& clientPortString, 
        const std::string& clientHostNameString);


    std::unordered_map<Index, sockaddr>& GetIndexAddrList()
    {
        return Index_Addresses;
    }

    //int getClientIndex(sockaddr addr)
    //{
    //    for (auto& IndexAddr: Index_Addresses)
    //    {
    //        //if the blocks of memory are the same
    //        //then return the index
    //        if (!memcmp(&IndexAddr.second, &addr, sizeof(addr)))
    //            return IndexAddr.first;
    //    }
    //}

//-------------------------------------------------------------------------


    //Call by internal function ReceiveMessage()
    //Simulate packet dropped rate based on config file
    //returns true if the message is successfully received from client
    //returns false if client has disconnected
    int udt_receive(
        char buffer[],
        size_t BUFFER_SIZE,
        sockaddr addr,
        int AddressSize);

//-------------------------------------------------------------------------


    //Receives Message from Client
    //returns true if the message is successfully received from client
    //returns false if client has disconnected
    int ReceiveMessage(
        int AddrIndex, 
        GameCommands& Command,
        std::vector<Message>& message)

    {

        //get client adress to send message to
        sockaddr addr = Index_Addresses[AddrIndex];
        int AddressSize = sizeof(addr);

        //Message Buffer size
        constexpr size_t BUFFER_SIZE = 1100;
        //Message buffer
        char buffer[BUFFER_SIZE];

        //The total message from the Client
        char messageFromClient[BUFFER_SIZE] = "\0";


        char command = -1;
        int LengthOfPayload = -1;

        //Calculate the total number of bytes received from the Client
        int Total_bytesReceived = 0;

        bool CheckedPayLoadLength = false;

        //Continue receiving bytes from client
        while (true)
        {
            //blocking call that receives bytes from the clientSocket
            const int bytesReceived = udt_receive(
                buffer, BUFFER_SIZE - 1,
                addr, AddressSize);

            //if bytesReceived is -1 then there is a socket error
            //client may be disconnected from Server
            if (bytesReceived == SOCKET_ERROR)
            {
                //stops the receiving loop from the client
               // std::cerr << "recv() failed." << std::endl;
                //break;
                return 0;
            }

            //if the numbeer of bytes received is 0
            //then break the receiving loop
            //client has asked for disconnection from server
            if (bytesReceived == 0)
            {
                //std::cerr << "Graceful shutdown." << std::endl;
                //break;
                return 2;
            }

            //manually null terminate the string
            buffer[bytesReceived] = '\0';


            //adds buffer string to the total message string
            memcpy(messageFromClient + Total_bytesReceived, buffer, bytesReceived);

            //Calculate the total number of bytes received from the Client
            Total_bytesReceived += bytesReceived;

            // Message format : Command(1byte), TotalMessLength(4bytes), Payload(), Null character(1byte)
            //     - Minimum number of bytes in Message is 5 + 1 = 6 bytes.
            //     - Message should be in const char*, and not string format.
            //     - Segment's Payload() should not exceed 1024 bytes.
            if (Total_bytesReceived >= 5 && !CheckedPayLoadLength)
            {
                CheckedPayLoadLength = false;

                const char* messageCharArray = messageFromClient;

                //Extract the command (1bytes)
                memcpy(&command, messageCharArray, sizeof(command));

                //Extract the LengthOfPayload (4bytes)
                memcpy(&LengthOfPayload,
                    messageCharArray + sizeof(command), sizeof(LengthOfPayload));
                //convert network byte to host byte
                LengthOfPayload = ntohl(LengthOfPayload);

                //remove the command & LengthOfPayload bytes from the Message
                     //to get the MessagePayLoad
                memcpy(messageFromClient,
                    messageFromClient + sizeof(command) + sizeof(LengthOfPayload),
                    Total_bytesReceived - sizeof(command) - sizeof(LengthOfPayload));


                size_t Payload = Total_bytesReceived - sizeof(command) - sizeof(LengthOfPayload);/*Null character(1byte)*/
                //if already received all the bytes in the message from the client
                //then break out of the receiving message loop
                if (Payload == LengthOfPayload)
                    break;

                //the remaining messageFromClient bytes
                Total_bytesReceived -= sizeof(command) - sizeof(LengthOfPayload);
            }
            //check if already received all the bytes in the message from the client
            else if (Total_bytesReceived >= 5)
            {
                //then break out of the receiving message loop
                if ((Total_bytesReceived - sizeof('\0')) == LengthOfPayload)
                    break;
            }

        }

        messageFromClient[LengthOfPayload] = '\0';

        Command = (GameCommands)command;

        size_t offset_size = 0;
        LengthOfPayload -= 1; //for nullchar
        //parse payload into a vector of Messages
        while (LengthOfPayload)
        {
            short size = 0;
            //Copy size of message into MessageBuffer
            memcpy_s((void*)&size,
                sizeof(short),
                messageFromClient + offset_size,
                sizeof(short));
            //the total offset size
            offset_size += sizeof(short);
            //left over length of payload
            LengthOfPayload -= sizeof(short);
            //convert host byte to network byte
            size = htons(size);


            //Message Buffer size
            constexpr size_t BUFFER_SIZE = 1100;
            //Message buffer
            char buffer[BUFFER_SIZE];
            if (size)
            {
                //Copy message into MessageBuffer
                memcpy_s(buffer,
                    size,
                    messageFromClient + offset_size,
                    size);

                //the total offset size
                offset_size += size;
                //left over length of payload
                LengthOfPayload -= size;

                message.push_back({ buffer, (size_t)size });
            }
        }

        return 1;

    }


    //sends the ClientMessage to the Server
    //returns true if the message is successfully sent to Server
    //returns false if Server has disconnected
    bool SendClientMessage(
        int AddrIndex, 
        const Message& message)
    {

        //get client adress to send message to
        sockaddr addr = Index_Addresses[AddrIndex];
        int AddressSize = sizeof(addr);

        //require to attach this client address to inform
        //the other client who is sending the meessage?

        int TotalBytesSent = 0;
        while (TotalBytesSent != (message.size_))
        {
            const int bytesSent = sendto(clientUDPSock_,
                message.message + TotalBytesSent,
                (int)message.size_ - TotalBytesSent, 0,
                &addr, AddressSize);

            TotalBytesSent += bytesSent;
            //failed to send bytes to the client
            if (bytesSent == SOCKET_ERROR)
            {
                //std::cerr << "send() failed." << std::endl;
                return false;
                //break;
            }
        }
        return true;
    }


    //send the message to all clients
    void BroadcastMessage(const Message& message, int index = -1)
    {
        if (index == -1)
        {

            //send the message to all clients
            for (auto& address : Index_Addresses)
            {
                SendClientMessage(
                    address.first, message);
            }
        }
        else
        {
            for (auto& address : Index_Addresses)
            {
                if (address.first == index)
                {
                    SendClientMessage(
                        address.first, message);
                    break;
                }
            }
        }
    }

 //-------------------------------------------------------------------------

    //Shutdown and close clientSocket
    void disconnect();


    //Clean up the Winsock Application
    void Cleanup();
};

#endif