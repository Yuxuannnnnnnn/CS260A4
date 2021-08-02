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

	//no need to be thread safe
	//gameObjects container not related to communication with the other packets
	std::vector<gameObjectID> DeletionList;
	

//-------------helper functions for GameObjects------------------------


	//When host client Spawns the gameObjects
	//host client will send to all other clients the list of gameObjects spawned
	//All other clients will insert the list of gameObjects
	void insert_GameObjects_DataInitialisation()
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

