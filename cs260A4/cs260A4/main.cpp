// cs260A4.cpp : Defines the entry point for the application.

#include "core/Game.h"
#include <memory>


//Wrapper to parse the commandline from the console
#include "Tools/Console.h"


#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine, //commandline
	_In_ int       nCmdShow)
{
	//constructor open up console
	Console console;

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
			cmdParser.Get_HostName_Port_List());

	//run the game
	game->Run();

	return 0;
}