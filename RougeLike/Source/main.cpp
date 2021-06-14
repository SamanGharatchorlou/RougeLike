#include "pch.h"


#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Game/GameController.h"
#include "Debug/PerformanceProfiler.h"

#pragma comment(lib, "SDL2_image")

#include <thread>
#include <iostream>

#include "Networking/Client.h"
#include "Networking/Server.h"


int main(int argc, char* args[])
{
	GameController GameApp;

	GameApp.init();
	GameApp.preLoad();

	GameApp.load();
	                                                                                                                                          
	GameApp.run();

	GameApp.free();

	PRINT_MEMORY;

	return 0;
}
