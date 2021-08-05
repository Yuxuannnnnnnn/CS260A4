#pragma once
#include "GameObject.h"
#include "../Math/Vector.h"
#include <vector>
#include <array>
#include <unordered_map>

class Factory
{
public:

	//storage of gameObjects
	//cannot use array because stack might not have enough space
	//need to use heap memory or virtual memory
	typedef int gameObjectID;
	std::unordered_map<gameObjectID, GameObject> gameObjects;
	gameObjectID counterID{0};


	//no need to be thread safe
	//gameObjects container not related to communication with the other packets
	std::vector<gameObjectID> DeletionList;

	//store this player gameObjectID
	gameObjectID _playerObjectID;


	//list of playersID and gameObjectID
	typedef int playerID;
	std::unordered_map<playerID, gameObjectID> playerObjectsList;

	//-------------helper functions for GameObjects------------------------

	//for logicSystem to get the player GameObject
	GameObject& getOwnPlayer()
	{
		return gameObjects[_playerObjectID];
	}

	//for logicSystem to get any opponent player GameObject
	GameObject& getPlayer(playerID ID)
	{
		return gameObjects[playerObjectsList[ID]];
	}


	//creation of playerShips at the start of the game 
	//different ships have different colors
	const std::unordered_map<int, gameObjectID>&  Create_playerShips_DataInitialisation(
		std::vector<int> indicesList, 
		int playerID)
	{
		//create own ship - blue color
		playerObjectsList.insert({ playerID, _playerObjectID = CreateShip(playerID, { 0.f, 0.f, 1.f }) });

		std::array<Vector3, 3> colorList;
		colorList[0] = { 1.0f, 0.f, 0.f };
		colorList[1] = {0.f, 1.0f, 0.f};
		colorList[2] = {1.0f, 1.0f, 0.f};

		for (size_t i = 0; i < indicesList.size(); i++)
		{
			//create all other clients ship & assign the indices to them
			playerObjectsList.insert({ indicesList[i], CreateShip(indicesList[i], colorList[i]) });
		}

		return playerObjectsList;

	}

	//returns gameObject ID
	gameObjectID CreateShip(int playerID, Vector3 color)
	{
		gameObjects[counterID] = GameObject{};
		gameObjects[counterID].transform.position = { 0.f, 0.f };
		gameObjects[counterID].transform.scale = { 20.f, 20.f };
		gameObjects[counterID].transform.rotation = 0.f;
		gameObjects[counterID].mesh = MeshType::triangle;
		gameObjects[counterID].color = color;
		gameObjects[counterID].playerIndex = playerID;

		counterID++;

		return counterID - 1;
	}


	//Every client will spawn asteroid
	// They will broadcast to everyone the asteroid positions & velocity
	// 
	//All other clients will insert the list of gameObjects
	void Asteroids_DataInitialisation( )
	{

	}




	//SYSTEMS to call this when they want to delete a gameObject
	void DeleteGameObjectID(gameObjectID id)
	{
		DeletionList.push_back(id);
	}

	//Update factory at the end of loop
	void update()
	{
		//updates the list of gameObjects is DeletionList
		for (auto& deleteID : DeletionList)
		{
			//erase from list by gameObject id
			gameObjects.erase(deleteID);
		}

		//clear the deletion list
		DeletionList.clear();
	}
};
