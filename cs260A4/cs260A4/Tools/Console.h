#pragma once
#include "../framework.h"
#include  <stdio.h>

class Console
{
public:

#ifdef DEBUG

	//open up the console for logging
	Console()
	{
		if (AllocConsole())
		{
			FILE* file;

			//open up streams for logging
			freopen_s(&file, "CONOUT$", "wt", stdout);
			freopen_s(&file, "CONOUT$", "wt", stderr);
			freopen_s(&file, "CONOUT$", "wt", stdin);

			SetConsoleTitle(L"Networking Game Console");
		}
	}
#else //not debug mode

	//init does nothing
	Console()
	{
	}

#endif
};
