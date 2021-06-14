#pragma once

#include <thread>

#include "NetworkData.h"


#include "Input/InputManager.h"

class Client;
class Server;

class InputManager;
class PlayerManager;
class FrameRateController;

class NetworkManager
{
public:
	NetworkManager();

	void listenForMessages();
	void sendMessages();

	bool mSetupComplete = false;
	Server* mServer = nullptr;
	Client* mClient = nullptr;

	std::thread* mThread = nullptr;

	int handleNetworkInput();

	InputManager* input;
	FrameRateController* frameRateController;
	PlayerManager* netoworkPlayer;

private:
	InputManager networkInput;

};


InputPacket generateInputPacket(const InputManager* input);

void listening(Server* server);
void sending(Client* client);
