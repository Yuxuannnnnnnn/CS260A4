#pragma once


enum class ApplicationCommands
{
//--------------------------------------from server to client / client to server----------------------------------
	NotifyServerSharedFiles,// - "Notification" from client 		client command(1byte), 
							//
							//										TotalMessLength(4bytes),
							//										Addresslength(2bytes),	  IP address of client1(...),
							//										portNumberLength(2bytes), Available UDP portNumber of client
							//																(2 bytes - enough to hold 0 to 655335 port numbers).
							// 										fileNameLength(2bytes), FileName(...),
							// 										fileNameLength(2bytes), FileName(...),
							//										nullcharacter(1 byte)

	RequestList, 			// - "RequestList" from client          Client command(1byte),
							//
							//										TotalMessLength(4bytes),
							//									    nullcharacter(1 byte)

	SendList,				//	- "SendList" from server			Server command(1byte),
							//
							//										TotalMessLength(4bytes),
							//										fileNameLength(2bytes), fileName(...),
							//										fileNameLength(2bytes), fileName(...),
							//										nullcharacter(1 byte)

	RequestFile,			// - "RequestFile" fromClientToServer	Client command(1byte),
							//										TotalMessLength(4bytes),
							 
							//										Addresslength(2bytes),	  IP address of client1(...),
							//										portNumberLength(2bytes), Available UDP portNumber of client
							//																(2 bytes - enough to hold 0 to 655335 port numbers).
							
							// 										fileNameLength(2bytes), fileName(...),
							//										nullcharacter(1 byte)

	FileDoesNotExist,		// - "FileDoesNotExist" from server		Server command(1byte),
							//
							//										TotalMessLength(4bytes),
							//										nullcharacter(1 byte)

	Notification,			// - "Notification" from server 		Server command(1byte), 
							//
							//										TotalMessLength(4bytes),
							//										Addresslength(2bytes),	  IP address of client1(...),
							//										portNumberLength(2bytes), Available UDP portNumber of client
							//																(2 bytes - enough to hold 0 to 655335 port numbers).
							//										enumLength(2bytes),		client is File Sender(0) / Receiver(1) (1 byte),
							//										fileNameLength(2bytes), FileName(...),
							//										nullcharacter(1 byte)


	Quit,					// - "Quit" from server/client			Client/Server command(1byte),
							//
							//										TotalMessLength(4bytes),
							//										nullcharacter(1 byte)


//-------------------------------------------------from client to client---------------------------------------------------

	FileSize,				// - "FileSize" from sourceClient		Client command(1byte),
							//
							//										TotalMessLength(4bytes),
							//										starting packetNumberLength(2bytes), packet number(4 bytes) 
							//										fileDataLength(2bytes), FileSize(4bytes),
							//										nullcharacter(1 byte)
	

	SendFileData,			// - "SendFileData" from sourceClient	Client command(1byte),
							//
							//										TotalMessLength(4bytes),
							//										packetNumberLength(2bytes), packet number(4 bytes) //to be acknowledged
							//										fileDataLength(2bytes), file data(1024bytes-14bytes),
							//										nullcharacter(1 byte)
	
	Ack,					// - "Ack" from destination client		Client command(1byte),
							//
							//										TotalMessLength(4bytes),
							//										packetNumberLength(2bytes), packet number(4 bytes) //acknowledge the packet
							//										nullcharacter(1 byte)
	
	EndOfFile,				// - "EndOfFile" download 				Client command(1byte),
							//
							//										TotalMessLength(4bytes),
							//										nullcharacter(1 byte)


	//dont need anymore, the task for the destination & source client just never ends
	//ResumeFileSending		// - "ResumeFileSending" fromLastDiscon  Client command(1byte),
							//
							//									  	  TotalMessLength(4bytes),
							//									  //Own address & udp port for source client to know
							//									  //who to send message to
							//									  	  Addresslength(2bytes), IP address of own client(...), 
							//									  	  Available UDP portNumber of own client
							// 									  
							//									  	  (2 bytes - enough to hold 0 to 655335 port numbers)
							//									  	  fileNameLength(2bytes), fileName(...),
							//									  //need to know where is stopped
							//									  	  SizeOfDownloadedFile(4 bytes), 
							//									  	  nullcharacter(1 byte)

							//should be sent after clientreconnection,
								//when a client is started, client should read if tempfile exists, 
									//if tempfile exists, then client should ask for ResumeFileSendingfromLastDisconnection   
									//if dont exist then do nothing.


	TotalCommands

};

