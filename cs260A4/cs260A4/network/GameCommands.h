#pragma once


enum class GameCommands
{
//-------------------------------------------------from client to client---------------------------------------------------
	
	
	
	JoinGame,				//Just Joined the Game					Payload: no payload
	InGame,					//Already in the Game					Payload: no payload
	Waiting,
	
	MoveForward,
	MoveBackward,
	RotateLeft,
	RotateRight,
	Shoot,


	SendIndex,				//Broadcast PLayerIndex to everyone    Payload: OwnPlayerIndex


	//Host will broadcast to all players that the ships are created with the following settings
	PlayerShipsCreation,											//Payload: Color

	//Host will broadcast to all players that the asteroids are created with the following settings
	AsteroidsCreation,												//Payload: 

	//For Logic System usage
	SendAcceleration,



	//Example format
	//Command,				//										Client command(1byte),
	//						//
	//						//										TotalMessLength(4bytes),
	//						//										Length(2bytes), payload
	//						//										Length(2bytes), payload,
	//						//										nullcharacter(1 byte)

	Disconnected,
	TotalCommands

};

