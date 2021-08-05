#pragma once
#include "GameObject.h"
#include <vector>
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
	gameObjectID playerID;

	//list of players gameObjectID
	std::vector<gameObjectID> playerObjectsList;

	//-------------helper functions for GameObjects------------------------

	//for logicSystem to get the player GameObject
	GameObject getOwnPlayer()
	{
		return gameObjects[playerID];
	}


	//creation of playerShips at the start of the game 
	//at a position
	void Create_playerShips_DataInitialisation(std::vector<int> indicesList)
	{
		//create own ship
		
		//create all other clients ship & assign the indices to them

	}

	//Every client will spawn asteroid
	// They will broadcast to everyone the asteroid positions & velocity
	// 
	//All other clients will insert the list of gameObjects
	void Asteroids_DataInitialisation( )
	{
	}


	//returns gameObject ID
	int CreateShip()
	{
		gameObjects[counterID] = GameObject{};
		gameObjects[counterID].transform.position = { 0.f, 0.f };
		gameObjects[counterID].transform.scale = {20.f, 20.f};
		gameObjects[counterID].transform.rotation = 0.f;
		gameObjects[counterID].mesh = MeshType::triangle;
		gameObjects[counterID].color;

		counterID++;

		return counterID -1;
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
