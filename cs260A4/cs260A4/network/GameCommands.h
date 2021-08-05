#pragma once


enum class GameCommands
{
//-------------------------------------------------from client to client---------------------------------------------------
	JoinGame,				//Just Joined the Game					Payload: no payload
	InGame,					//Already in the Game					Payload: no payload
	MoveForward,
	MoveBackward,
	RotateLeft,
	RotateRight,
	Shoot,





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

