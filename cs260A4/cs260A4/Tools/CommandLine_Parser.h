#pragma once


#ifndef COMMANDLINE_PARSER
#define COMMANDLINE_PARSER

#include <Windows.h> //for windows functions, get cmd line
#include <memory> //for printf

#include <string>
#include <vector>

#include <shellapi.h>


#include "EngineSettings.h" //for ASSERT function


typedef std::string HostnameString;
typedef std::string PortString;
typedef  std::vector<std::pair<HostnameString, PortString>> Hostname_Port_List;


class CommandLine_Parser
{
	//parsed argument list
	//A pointer to an array of LPWSTR values, similar to argv.
	LPWSTR* szArglist{ nullptr };

	//number of arguments
	int nArgs = 0;


	//list of Pairs of HostName& Port strings
	Hostname_Port_List hostname_Port_List;

	//helper function to if sufficient player arguments
	bool CheckAppropriateArguments()
	{
		//arguments supposed to be the following
		//1 >game.exe  hostname1:port1   hostname2:port2   hostname3:port3   hostname4:port4

		//the number of players indicated are neither 3 or 4
		if (nArgs != 5 && nArgs != 4)
		{
			printf("Not enough players indicated in the commandline!\n\n");
			return false;
		}

		return true;
	}
public:

	//-----------------------------------------------------------------------------------
		//return false when either not enough players
		//or player has no PortNumber or HostName
	bool Parse()
	{
		//CommandLineToArgvW parse a Unicode command-line string
		// (such as lpCmdLine) to
		//an argv-style array of Unicode strings.
		//also get the number of arguments in the commandline
		szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);

		//If the CommandLineToArgvW function fails, the return value is NULL.
		if (NULL == szArglist)
		{
			//print in debug mode, else do nothing
			ASSERT("CommandLineToArgvW failed\n");
		}
		else
		{
			//print each argument
			printf("Commandline Arguments: \n");

			for (int i = 0; i < nArgs; i++)
				printf("%d: %ws\n", i, szArglist[i]);

			printf("\n");
		}

		//allocate space in list first
		hostname_Port_List.reserve((size_t)(nArgs - 1));

		//converts arguments into hostName strings & Port Strings
		//1 >game.exe  hostname1:port1   hostname2:port2   hostname3:port3   hostname4:port4
		//start from 1, index 0 is the exe
		for (int i = 1; i < nArgs; i++)
		{
			// convert wchar_t* to wstring
			std::wstring wstr(szArglist[i]);
			//convert wstring to string
			std::string str(wstr.begin(), wstr.end());

			//Seperate HostName & Port Strings
			auto position = str.find(":");
			//cannot find ":"
			if (position == std::string::npos)
			{
				printf("One of the players do not have PortNumber or HostName!\n\n");
				return false;
			}

			std::string HostnameString = str.substr(0, position);
			std::string PortString = str.substr(position + 1);

			//Store in List
			hostname_Port_List.push_back({ HostnameString , PortString });
		}

		return CheckAppropriateArguments();
	}

	//getter function
	//returns a list of Pairs of HostName & Port strings
	Hostname_Port_List& Get_HostName_Port_List()
	{
		return hostname_Port_List;
	}

	//destructor
	~CommandLine_Parser()
	{
		// Free memory allocated for CommandLineToArgvW arguments.
		LocalFree(szArglist);
	}
};


#endif