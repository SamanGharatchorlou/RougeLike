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

	// left = 1, right = 2, up = 3, down = 4 
	int mMovement = 0;

	int handleNetworkInput();

	InputManager* input;
	FrameRateController* frameRateController;
	PlayerManager* netoworkPlayer;

private:
	InputManager networkInput;

};


InputPacket getClientInput(const InputManager* input);

void listening(Server* server);
void sending(Client* client);
