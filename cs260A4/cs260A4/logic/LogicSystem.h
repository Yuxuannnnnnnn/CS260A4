#pragma once


#ifndef LOGICSYSTEM
#define LOGICSYSTEM


#include "../input/InputSystem.h"
#include "../input/KeyEnum.h"
#include "DRData.h"
#include "../core/GameObject.h"

#include <vector>
#include <mutex>
#include <functional>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>


#include <typeinfo> //For typeid() function

//for gameObjects creation 
//store factory pointer in LogicSystem
#include "../core/Factory.h"
#include "../core/Game.h"

//for event
#include "../network/MessageFormat.h"
#include "../network/GameCommands.h"


//for PRINTOUT function
#include "../Tools/EngineSettings.h"
#include <fstream>

#define PI 3.1415926f


struct Lock_Step_Struct
{
	int playerIndex;
	size_t storedHashValue;
	GameCommands storedCommand;
};
class WindowsSystem;

class LogicSystem
{
	struct KeyBind
	{
		KEY move_forward;
		KEY move_backward;
		KEY rotate_left;
		KEY rotate_right;
		KEY shoot;
	};
public:
	typedef std::vector<Message> MessageList;
	typedef std::pair<GameCommands, MessageList> Event;
	typedef int clientsAddressIndex;
	typedef std::pair<clientsAddressIndex, Event> clientIndex_Event_Pair ;
	//list of playerIndex & Event Pair
	//the logic system has to clear this eventsList every loop
	std::vector<clientIndex_Event_Pair> EventsList;
	std::mutex EventsList_Mutex;


	//store the vector index of the events in the EventsList to be deleted
	//Store the Events that should be deleted after pulled
	//after traversing the eventList
	std::vector<int> EventsDeletionList;


	//for logicSystem update to use 
	//callback function to insert a notification to the 
	//notification list in the networking system
	//Function to Send notifications to other clients
	typedef std::function<void(const GameCommands& command,
												//clientAddressIndex = -1 refers to send to everyone
		const std::vector<Message> messageList, int clientAddressIndex)>
		InsertNotificationFunction;
	InsertNotificationFunction _InsertNotification;


	//indicates the number of players in the Game
	//If the commandline has 4 addresses, the NumOfPlayersRequired will be 4
	int _NumOfPlayersRequired{ 0 };


	//stores the map to get the playerIndex from the clientsAddressIndex
	typedef int playerID;
	std::unordered_map<clientsAddressIndex, playerID> clientAddrID_PlayerID_List;
	//get the clientAddrIndex of the host
	clientsAddressIndex hostAddrIndex;


	//boolean for whether this player is a host
	//The client that is the host
	//will define the settings of the asteroids throughout the game & 
	//and the initial settings of all the player ships
	bool _isHost{ false };



	//Own player ID
	playerID _playerID{ 0 };

	//store factory - for game objects creation
	Factory* gameFactory;

	bool isAlive = true;
	/*
	*	lock-step protocol
	*/
	
	// player's commitment

	void* committment;
	// playernumber , hashed commitment
	//        0     ,   ........
	//        1     ,   ........
	//        2     ,   ........
	//        3     ,   ........
	std::map<playerID, void*> other_commitments;

	// wait till everyone ready
	bool _islocking;
	float _waitingTime = 0;
	static constexpr float locksteptime = 0.3f;

	// use key from keybind.cfg file
	bool _useCustKey = false;
	KeyBind _keybind;

	void Lock_Step(GameCommands gamecommands);

	// encrypt gamecommand using hash
	size_t Hash(GameCommands gamecommands);
public:

//--------------------Initialise the LogicSystem------------------------------------

	void Init(Factory* factory,
		const InsertNotificationFunction& InsertNotification, 
		int NumOfPlayersRequired, bool useCustKey)
	{

		for (int i = 0; i < 4; ++i)
		{
			lockSteps[i].playerIndex = i;
		}

		//get function to insert notification to networkSystem
		_InsertNotification = InsertNotification;

		//If the commandline has 4 addresses, the NumOfPlayersRequired will be 4
		_NumOfPlayersRequired = NumOfPlayersRequired;

		//store factory - for game objects creation
		gameFactory = factory;

		_useCustKey = useCustKey;

		if (_useCustKey)
		{
			std::ifstream filein{ "keybind.cfg" };
			std::string str;

			int keyindex = 0;

			while (filein.good())
			{
				std::getline(filein, str);
				
				int key = std::stoi(str);

				switch (keyindex)
				{
					// forward
				case 0:
					_keybind.move_forward = (KEY)key;
					break;
					// backward
				case 1:
					_keybind.move_backward = (KEY)key;
					break;
					// rotateleft
				case 2:
					_keybind.rotate_left = (KEY)key;
					break;
					// rotateright
				case 3:
					_keybind.rotate_right = (KEY)key;
					break;
					// shoot
				case 4:
					_keybind.shoot = (KEY)key;
					break;
				}

				keyindex++;
			}
		}
	}

//--------------------Waiting for players to Join game------------------------------------

	void GameMenu(NetworkSystem* networkSystem);



	void Wait_ForAllPlayers(WindowSystem& _windowSystem,
		InputSystem& inputsystem);


	void clearEventsToBeDeleted()
	{
		std::sort(EventsDeletionList.begin(), EventsDeletionList.end(), std::greater<int>());
		for (auto& deleteEvent : EventsDeletionList)
		{
			EventsList.erase(EventsList.begin() + deleteEvent);
		}

		EventsDeletionList.clear();
	}

	//Configure if this client is a HostPlayer
	void HostPlayer(bool isHost)
	{
		_isHost = isHost;
	}


//-------------------------Host Initialisation of Game------------------------------------


	void HostInitGame()
	{
		//if this client is the host, then is the host responsibility
		//to define the settings of the asteroids throughout the game & 
		//and the initial settings of all the player ships
		if (_isHost)
		{
			MessageList messageList;

			auto& ShipList = 
				gameFactory->Create_playerShips_DataInitialisation(
				_NumOfPlayersRequired,
				_playerID);

			auto AsteroidsList = 
				gameFactory->Create_Asteroids_DataInitialisation();

			//players need to know the number of gameObjects in each message
			//push number of gameObjects
			Insert_Number_MessageList(messageList, ShipList.size());

			for (auto& object : ShipList)
			{
				//players need to know the gameObjectID of each gameObject
				//push gameObjectID
				Insert_Number_MessageList(messageList, object.second);

				//players need to know the details of each gameObjects
				GameObject& obj = gameFactory->getGameObject(object.second);
				InsertGameObject_MessageList(messageList, obj);
			}
			_InsertNotification(GameCommands::SynchronisePlayer, messageList, -1);


			int asteroidsCounter = 0;
			//send a message packet, 10 asteroids by 10 asteroids
			while (asteroidsCounter < AsteroidsList.size())
			{
				messageList.clear();

				//add new asteroids count to the asteroidsCounter
				int addNewAsteroids = (int)((AsteroidsList.size() - asteroidsCounter) < 5?
					(AsteroidsList.size() - asteroidsCounter) : 5);

				//players need to know the number of gameObjects in each message
				//push number of gameObjects
				Insert_Number_MessageList(messageList, addNewAsteroids);
				for (int i = 0; i < addNewAsteroids ; i++)
				{
					auto& object = AsteroidsList[asteroidsCounter + i];
					//players need to know the gameObjectID of each gameObject
					//push gameObjectID
					Insert_Number_MessageList(messageList, object);

					//players need to know the details of each gameObjects
					GameObject& obj = gameFactory->getGameObject(object);
					InsertGameObject_MessageList(messageList, obj);
				}

				_InsertNotification(GameCommands::SyncrhoniseAsteroids, messageList, -1);


				asteroidsCounter += addNewAsteroids;

			}

		}
	}



//Insert to MessageList---------------------

	void InsertDRData_MessageList(MessageList& messageList, const DRData& drdata)
	{
		Insert_Number_MessageList(messageList, drdata.accelx);
		Insert_Number_MessageList(messageList, drdata.accely);
		Insert_Number_MessageList(messageList, drdata.gametime);
		Insert_Number_MessageList(messageList, drdata.playerindex);
	}

	void InsertGameObject_MessageList(MessageList& messageList, const GameObject& object)
	{
		Insert_Vec2_MessageList(messageList, object.transform.position);
		Insert_Vec2_MessageList(messageList, object.transform.scale);
		Insert_Number_MessageList(messageList, object.transform.rotation);

		Insert_Vec2_MessageList(messageList, object.rigidbody.velocity);
		
		Insert_Number_MessageList(messageList, object.mesh);
		Insert_Vec3_MessageList(messageList, object.color);

		Insert_Number_MessageList(messageList, object.obj_type);
		Insert_Number_MessageList(messageList, object.playerIndex);
	}


	//function to convert a Vec3 to a string and then to a Message
	//Insert the Mesage to the messageList
	void Insert_Vec3_MessageList(MessageList& messageList, const Vector3& vec)
	{
		Insert_Number_MessageList(messageList, vec.x);
		Insert_Number_MessageList(messageList, vec.y);
		Insert_Number_MessageList(messageList, vec.z);
	}

	//function to convert a Vec2 to a string and then to a Message
	//Insert the Mesage to the messageList
	void Insert_Vec2_MessageList(MessageList& messageList, const Vector2& vec)
	{
		Insert_Number_MessageList(messageList, vec.x);
		Insert_Number_MessageList(messageList, vec.y);
	}


	//function to convert an integer/float to a string and then to a Message
	//Insert the Mesage to the messageList
	template<typename type>
	void Insert_Number_MessageList(MessageList& messageList, const type& number)
	{
		messageList.push_back({ std::to_string((float)number).c_str(),
		std::to_string((float)number).size() + sizeof("\0") });
	}

	template<typename type>
	void FrontInsert_Number_MessageList(MessageList& messageList, const type& number)
	{
		messageList.insert(messageList.begin(), {std::to_string(number).c_str(),
		std::to_string(number).size() + sizeof("\0") });
	}

//Extract From MessageList---------------------

	void ExtractDRData_MessageList(int index, MessageList& messageList, DRData& drdata)
	{
		Extract_Number_MessageList(index, messageList, drdata.accelx);
		Extract_Number_MessageList(index + 1, messageList, drdata.accely);
		Extract_Number_MessageList(index + 2, messageList, drdata.gametime);
		Extract_Number_MessageList(index + 3, messageList, drdata.playerindex);
	}

	void ExtractGameObject_MessageList(int index, MessageList& messageList, GameObject& object)
	{
		Extract_Vec2_MessageList(index, messageList, object.transform.position);
		Extract_Vec2_MessageList(index + 2, messageList, object.transform.scale);
		Extract_Number_MessageList(index + 4, messageList, object.transform.rotation);

		Extract_Vec2_MessageList(index + 5, messageList, object.rigidbody.velocity);

		Extract_Number_MessageList(index + 7, messageList, object.mesh);
		Extract_Vec3_MessageList(index + 8, messageList, object.color);

		Extract_Number_MessageList(index + 11, messageList, object.obj_type);
		Extract_Number_MessageList(index + 12, messageList, object.playerIndex);
	}




	//function to extract an vec3 from the messageList
	void Extract_Vec3_MessageList(int index, MessageList& messageList, Vector3& vec)
	{
		Extract_Number_MessageList(index, messageList, vec.x);
		Extract_Number_MessageList(index + 1, messageList, vec.y);
		Extract_Number_MessageList(index + 2, messageList, vec.z);
	}

	//function to extract an vec2 from the messageList
	void Extract_Vec2_MessageList(int index, MessageList& messageList, Vector2& vec)
	{
		Extract_Number_MessageList(index, messageList, vec.x);
		Extract_Number_MessageList(index + 1, messageList, vec.y);
	}


	//function to extract an integer or float from the messageList
	template<typename type>
	void Extract_Number_MessageList(int index, MessageList& messageList, type& number)
	{
		Message mess = messageList[index];
		char buffer[100] = "\0";
		memcpy(&buffer, mess.message, mess.size_);

		std::string NumberString = buffer;

		if (typeid(type) == typeid(int) || typeid(type) == typeid(MeshType) ||
			typeid(type) == typeid(TYPE))
		{
			number = (type)stoi(NumberString);
		}
		else if (typeid(type) == typeid(float))
		{
			number = (type)stof(NumberString);
		}
	}


	


//--------------------------------In the GameLoop------------------------------------


	//when looping through the eventslist in the update
	//should lock the mutex before entering the loop
	void Update(const InputSystem& inputsystem, float dt, float gametime, Factory*);

	// testupdate
	void TestUpdate(const InputSystem& inputsystem, float dt, std::vector<GameObject>& gameobjlist);

//----------------------Functions For NetworkSystem to use------------------------------------

	//a function for the NetworkSystem to use
	//when the networkSystem received a packet
	//from the other players
	//inserts an Event for the logicSystem to process
	void InsertEvent(
		int clientAddressIndex, 
		const Event& _event)
	{
		std::lock_guard<std::mutex> EventsListLock{ EventsList_Mutex };
		//stores the event in the EventsList
		EventsList.push_back({ clientAddressIndex,_event });
	}

	

private:
	// number of point scored, each client has 1
	int _score = 0;
	static constexpr int asteriod_reward = 10;

	int _loopCounter = 0;
	int _synCount = 10;

	void PullEvent(float currgametime, Factory*);
	void SynchronisePosition(Factory*);
	void PerformDR(GameObject& ship, const DRData& drdata, float currgametime);
	bool CollisionIntersection(const AABBCollider& aabb1, const AABBCollider& aabb2);
	void CheckCollision(Factory*);

	float Wrap(float x, float x0, float x1);
	float acceleration_speed = 40.0f;
	float rotation_speed = 6.3f;
	static constexpr float bulletspeed = 200.0f;

	float wCoolDown;
	float sCoolDown;

	size_t storedHashValue;
	GameCommands storedCommand;
 

	Lock_Step_Struct lockSteps[4];

};




#endif