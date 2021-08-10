// cs260A4.cpp : Defines the entry point for the application.



#include "core/Game.h"
#include <memory>


//Wrapper to parse the commandline from the console
#include "Tools/Console.h"

//#include "core/testgame.h"
#include "core/GameObject.h"
#include "physics/Transform.h"
#include "physics/Rigidbody.h"
#include <vector>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine, //commandline
	_In_ int       nCmdShow)
{
	//constructor open up console
	Console console;
	/*  test graphics code, delete later
		TEST BEGIN
		*************************
		KEY : W A S D SPACE


		REMOVE THE LINE BELOW (LINE 36) TO COMMENT OUT
	*/
	/*
	Transforms trans{ {-50,0},0,{25,15} };

	std::vector<GameObject> objlist;
	objlist.push_back(GameObject{ trans, Rigidbody{}, TYPE_PLAYER, triangle, 0 });

	trans.position.x = -20;

	objlist.push_back(GameObject{ trans, Rigidbody{}, TYPE_PLAYER, triangle, 1 });

	trans.position.x = 10;

	objlist.push_back(GameObject{ trans, Rigidbody{}, TYPE_PLAYER, triangle, 2 });

	trans.position.x = 40;

	objlist.push_back(GameObject{ trans, Rigidbody{}, TYPE_PLAYER, triangle, 3 });
	
	trans.rotation = 0;*/
	//for (int i = 0; i < 4; ++i)
	//{
	//	srand(time(NULL));
	//
	//	trans.position = { rand() % (50 + i) + 50.0f,  rand() % (50 - i) + 50.0f };
	//
	//	float scal = rand() % 50 + 30;
	//	trans.scale = { scal,scal };
	//	Rigidbody rb{ Vector2{ 35.0f + i, 30.0f - i} };
	//	if (i % 2 == 0)
	//		rb.velocity.x = -35;
	//
	//	objlist.push_back(GameObject{ trans, rb, TYPE_ASTEROID });
	//}
	//
	//
	//std::unique_ptr<TestGame> testgame =
	//	std::make_unique<TestGame>
	//	(hInstance, nCmdShow,
	//		WINDOW_WIDTH, WINDOW_HEIGHT, objlist);


	//testgame->Run();
	//
	//
	//return 0;

	/*
			TEST END
		*******************************************
	*/
	//parse the comandline arguments
	CommandLine_Parser cmdParser;
	bool check = cmdParser.Parse();
	if (!check)
		return 1;

	//create a game
	std::unique_ptr<Game> game =
		std::make_unique<Game>
		(hInstance, nCmdShow,
			WINDOW_WIDTH, WINDOW_HEIGHT,
			cmdParser.Get_HostName_Port_List(), false); // true, use key from keybind.cfg

	//run the game
	game->Run();

	return 0;
}