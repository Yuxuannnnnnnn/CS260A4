#include "ClientUDPSocket.h"
#include "../Tools/EngineSettings.h"


/*******************************************************************************
 * A simple UDP/IP client application
 ******************************************************************************/




clientUDPSocket::clientUDPSocket(const clientUDPSocket& Socket) :
    clientUDPSock_{ Socket.clientUDPSock_ }, 
    RateOfPacketLoss_{ Socket.RateOfPacketLoss_}
{

}

//Starts up the Winsock application
//returns errorCode of WSAStartup()
int clientUDPSocket::Startup()
{
    // -------------------------------------------------------------------------
    // Start up Winsock, asking for version 2.2.
    //
    // WSAStartup()
    // -------------------------------------------------------------------------

            // This object holds the information about the version of Winsock that we
            // are using, which is not necessarily the version that we requested.
    WSADATA wsaData{};

    // Initialize Winsock. You must call WSACleanup when you are finished.
    // As this function uses a reference counter, for each call to WSAStartup,
    // you must call WSACleanup or suffer memory issues.
    int errorCode = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (errorCode != NO_ERROR)
    {
        std::cerr << "WSAStartup() failed." << std::endl;
        return errorCode;
    }

    //std::cout
    //	<< "Winsock version: "
    //	<< static_cast<int>(LOBYTE(wsaData.wVersion))
    //	<< "."
    //	<< static_cast<int>(HIBYTE(wsaData.wVersion))
    //	<< "\n"
    //	<< std::endl;

    return NO_ERROR;
}


//Resolve ServerHostName & portNumber into socket addresses
//Create ClientSocket and Connect to Server
//returns errorCode of getaddrinfo()/socket()/connect()
int clientUDPSocket::Prep_UDPClientSocket(     
    const std::string& clientPortString)     // own Port number - from config file
{

    clientPortString_ = clientPortString;

    struct hostent* ent = gethostbyname("localhost");
    struct in_addr ip_addr = *(struct in_addr*)(ent->h_addr);
    clientUDP_HostNameString_ = inet_ntoa(ip_addr);



    //------------------------------------------------
    // Resolve a server host name into IP addresses
    // (in a singly-linked list)
    // 
    // Get server IP address 
    // for connection of socket to the server IP address
    // 
    // getaddrinfo()
    //------------------------------------------------

    //Object hints indicates which protocols to use to fill 
    //in the info.
    addrinfo hints{};
    SecureZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET; //IPv4
    // For TCP use SOCK_STREAM instead of SOCK_DGRAM.
    hints.ai_socktype = SOCK_DGRAM;		// Best effort
    // Could be 0 for autodetect, but best effort over IPv4 is always UDP.
    hints.ai_protocol = IPPROTO_UDP;	// UDP

    addrinfo* info = nullptr;

    //Parameters of getaddrinfo
    // host (node) name or a numeric host address string. 
    //followed by Client port number, 
    int errorCode = getaddrinfo(nullptr, clientPortString.c_str(), 
                        &hints, &info);


    if (errorCode || (info == nullptr))
    {
        //std::cerr << "getaddrinfo() failed."
        //    << std::endl;
        //std::cerr << "No server listening at a given host or port."
        //    << std::endl;
        WSACleanup();
        return errorCode;
    }

    // -------------------------------------------------------------------------
    // Create a socket and bind it to own network interface controller.
    //
    // socket()
    // bind()
    // -------------------------------------------------------------------------

    clientUDPSock_ = socket(info->ai_family,
        info->ai_socktype, info->ai_protocol);



    //   This option is needed on the socket in order to be able to receive broadcast messages
    //   If not set the receiver will not receive broadcast messages in the local network.
    char broadcast = '1';
    if (setsockopt(clientUDPSock_, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
    {

        std::cout << "Error in setting Broadcast option" << std::endl;
        closesocket(clientUDPSock_);
        return 0;
    }

    //set address to send for broadcasting
    Broadcast_Sending_addr.sin_family = AF_INET;
    short port = stoi(clientPortString);
    Broadcast_Sending_addr.sin_port = htons(port);
    Broadcast_Sending_addr.sin_addr.s_addr  = INADDR_BROADCAST; // this isq equiv to 255.255.255.255


    if (clientUDPSock_ == INVALID_SOCKET)
    {
        //std::cerr << "socket() failed"
        //    << std::endl;
        freeaddrinfo(info);
        WSACleanup();
        return 2;
    }

    struct sockaddr_in Recv_addr;
    Recv_addr.sin_family = AF_INET;
    Recv_addr.sin_port = htons(port);
    Recv_addr.sin_addr.s_addr = INADDR_ANY;
    //bind socket to network interface controller 
    errorCode = bind(
        clientUDPSock_,
        (sockaddr*)&Recv_addr,//info->ai_addr,
        sizeof(Recv_addr));//static_cast<int>(info->ai_addrlen));



    if (errorCode != NO_ERROR)
    {
        //std::cerr << "bind() failed." << std::endl;
        closesocket(clientUDPSock_);
        freeaddrinfo(info);
        WSACleanup();
        return 2;
    }

    freeaddrinfo(info);


    return NO_ERROR;

}


//get and store the clientAddress to send/recv data from
    //if fail to get the clientAddress, then return -1
    //if succeed to get the clientAddress, then return the client index
int clientUDPSocket::Get_Store_ClientAddress(
    //Port number & IPaddress/HostName of client 
    //to send/recv data from
    const std::string& clientPortString,
    const std::string& clientHostNameString)
{
    //Object hints indicates which protocols to use to fill 
    //in the info.
    addrinfo hints{};
    SecureZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET; //IPv4
    // For TCP use SOCK_STREAM instead of SOCK_DGRAM.
    hints.ai_socktype = SOCK_DGRAM;		// Best effort
    // Could be 0 for autodetect, but best effort over IPv4 is always UDP.
    hints.ai_protocol = IPPROTO_UDP;	// UDP


    addrinfo* serverInfo = nullptr;
    int errorCode = getaddrinfo(
        clientHostNameString.c_str(), 
        clientPortString.c_str(), 
        &hints, 
        &serverInfo);
    if ((errorCode) || (serverInfo == nullptr))
    {
        //std::cerr << "getaddrinfo() failed." << std::endl;
        WSACleanup();
        return -1;
    }

    //get a client index to keep track
    int clientIndex = AddressIndex;
    AddressIndex++; //increment the Address Index

    //Store the client address by index
    Index_Addresses[clientIndex] = *serverInfo->ai_addr;
    Index_HostnamePort_List[clientIndex] = { clientPortString , clientHostNameString };


    char buffer[100] = "\0";

    std::cout << "clientPortString: " << clientPortString << std::endl;
    std::cout << "clientHostNameString: " << clientHostNameString << std::endl;
    std::cout << "Storing Address: "
        << inet_ntop(AF_INET, &Index_Addresses[clientIndex],
            buffer, 100)
        << ". " << std::endl;

    //char s[INET6_ADDRSTRLEN];
    //inet_ntop(hints.ai_family, ((struct sockaddr_in*)((struct sockaddr*)hints.ai_addr))->sin_addr, s, sizeof (s));

    uint16_t port = 0;
   auto ip =  print_ipv4(&Index_Addresses[clientIndex], port);
   PRINTOUT("host port: ", port, " ip: ", ip, "\n");

   delete[] ip;

    //the addrinfo is no longer needed thus free the info
    freeaddrinfo(serverInfo);


    //if succeed to get the clientAddress, then return the client index
    return clientIndex;
}



//Receives Message from Client
//Simulate packet dropped rate based on config file
//returns true if the message is successfully received from client
//returns false if client has disconnected
int clientUDPSocket::udt_receive(
    char buffer[], 
    size_t BUFFER_SIZE, 

//get the client address to check which client
//has sent the message
    sockaddr& addr, 
    int AddressSize)
{

    //Initialize random number generator
    //The pseudo - random number generator is
        //initialized using the argument passed as seed.
    srand((unsigned)time(NULL));
    //convert rate [0, 1] to [0, 100]

    //100% chance - no packets is dropped
    int rate = 0;//(int)(RateOfPacketLoss_ * 100.0f);


    //blocking call that receives bytes from the clientSocket
    int bytesReceived = recvfrom(clientUDPSock_,
        buffer, (int)BUFFER_SIZE - 1,
        0,
        &addr,
        (int*)&AddressSize);
    //get a chance from 0 to 100
    int chance = rand() % 100;


    //if chance is below the rate provided by the config file
    //then drop the packet, and receive the nezt packet
    while (chance < rate)
    {
        //blocking call that receives bytes from the clientSocket
        bytesReceived = recvfrom(clientUDPSock_,
            buffer, (int)BUFFER_SIZE - 1, 
            0,
            &addr,
            &AddressSize);


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

        //recalculate chance for this packet 
            //to be dropped
        chance = rand() % 100;
    }

    return bytesReceived;
 
}




//close clientSocket
//dont need to shutdown because 
//udp doesnt need 4-way handshake
void clientUDPSocket::disconnect()
{
    if (clientUDPSock_ != INVALID_SOCKET)
    {
        closesocket(clientUDPSock_);
        clientUDPSock_ = INVALID_SOCKET;
    }
}


//Clean up the Winsock Application
void clientUDPSocket::Cleanup()
{
    // -------------------------------------------------------------------------
    // Clean-up after Winsock.
    //
    // WSACleanup()
    // -------------------------------------------------------------------------

    WSACleanup();
}
