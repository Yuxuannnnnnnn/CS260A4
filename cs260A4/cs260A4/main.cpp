// cs260A4.cpp : Defines the entry point for the application.

#include "core/Game.h"
#include <memory>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{


    // TODO: Init Network System, wrapp outside the game or inside?


    std::unique_ptr<Game> game = std::make_unique<Game>(hInstance, nCmdShow, WINDOW_WIDTH, WINDOW_HEIGHT);

    while (game->GameIsRunning())
    {
        game->Update();
    }

}
