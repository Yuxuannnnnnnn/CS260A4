#pragma once


enum class GameCommands
{
//-------------------------------------------------from client to client---------------------------------------------------
	
	
	
	JoinGame,				//Just Joined the Game					Payload: no payload
	InGame,					//Already in the Game					Payload: no payload
	SendIndex,				//Broadcast PLayerIndex to everyone    Payload: OwnPlayerIndex


	//Host will broadcast to all players that the ships are created with the following settings
	//Host will broadcast to all players that the asteroids are created with the following settings
	GameObjects_Initialisation,								//Payload: numberOfGameObjects, 
															//		   GameObjectID
															//         Each GameObject Details


	//Each player will send their own Ship GameObject Details
	SynchronisePlayer,
	//Host will broadcast to all players the asteroids updated Details
	SyncrhoniseAsteroids,

	// to send synchroise, send object ID, gameobject data

	// to receive synchronse, send object ID, gameobject data
	Synchronise_Asteroids,
	Synchronise_Player,



	MoveForward,
	MoveBackward,
	RotateLeft,
	RotateRight,
	Shoot,


	//For Logic System usage
	DeleteAsteriod,
	PlayerDied,

	//For Lock-Step
	
	// initialiate an lockstep
	InitLockStep,
	// reply lockstep
	ReplyLockStep,
	SynchronizeAction,
	AnnounceAction,



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

