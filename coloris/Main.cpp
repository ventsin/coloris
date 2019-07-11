/*
 * Main.cpp - Entry point for Coloris application
 *
 */

//Pragma to disable Console integration
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

//Standard Library headers
#include <string>
#include <vector>
#include <iostream>

//Route headers
#include "Gui.hpp"

//Entry point
int main(int _argc, char** _argv)
{
	//Parse argv
	std::vector<std::string> argv;
	{
		for (int i = 0; i < _argc; ++i)
			argv.push_back(_argv[i]);
	}

	//Decide program path
	int ret = 0;
	{
		auto it = std::find(argv.begin(), argv.end(), "--console");
		if (it != argv.end())
		{
			//Console flag was set, pass argv to terminal route

		}
		else
		{
			//No "--console" flag was present, start in GUI mode
			ui::run();
		}
	}

	//Return
	return ret;
}