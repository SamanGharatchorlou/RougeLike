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

};

void listening(Server* server);