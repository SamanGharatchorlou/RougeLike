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
	int loops = 5;
	PerformanceProfiler profiler("Loading");
	for (int i = 0; i < loops; i++)
	{
		GameController GameApp;

		profiler.restart();

		GameApp.init();
		GameApp.preLoad();

		profiler.restart();

		GameApp.load();

		profiler.saveToAverage();
		profiler.displayTimeSeconds();

		//GameApp.run();

		GameApp.free();
	}

	profiler.displayAverageTimeSeconds();

	//GameController GameApp;

	//GameApp.init();
	//GameApp.preLoad();

	//PerformanceProfiler profiler("Loading");
	//profiler.start();

	//GameApp.load();

	//profiler.displayTimeSeconds();
	//                                                                                                                                          
	//GameApp.run();

	//GameApp.free();

	PRINT_MEMORY;


	//char connectionType[100];

	//gets_s(connectionType, 100);

	//if (strcmp(connectionType, "server") == 0)
	//{
	//	Server server;
	//	server.open();

	//	while (true)
	//	{
	//		BasicString message("", 1024);
	//		BasicString senderInfo("", 1024);

	//		server.receiveMessage(message, nullptr);

	//		if (!message.empty())
	//		{
	//			DebugPrint(Log, "%s\n", message.c_str());
	//		}
	//	}
	//}
	//else
	//{
	//	Client client;
	//	client.open();

	//	while (true)
	//	{
	//		BasicString message("", 1024);
	//		std::cin.get(message.buffer(), message.bufferLength());

	//		message.calculateLength();
	//		client.sendMessage(message);
	//		
	//		std::cin.ignore();
	//	}
	//}

	return 0;
}

