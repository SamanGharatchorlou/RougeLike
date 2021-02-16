#pragma once

#include <thread>

#include "Networking/Client.h"
#include "Networking/Server.h"


class NetworkManager
{
public:
	void listenForMessages();
	void sendMessages();

	bool mSetupComplete = false;
	Server* mServer = nullptr;
	Client* mClient = nullptr;

	std::thread* mThread = nullptr;

	// left = 1, right = 2, up = 3, down = 4 
	int mMovement = 0;

	int handleNetworkInput();


};

void listening(Server* server);