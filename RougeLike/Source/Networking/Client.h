#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

#include "NetworkData.h"

class InputManager;
class FrameRateController;

class Client
{
public:
	Client();

	void open();

	void receiveMessage(BasicString& outMessage, BasicString* senderInfo);

	void sendMessage(const BasicString& message);
	void sendMessage(NetworkInputData& data);


	void close();

	bool sending = true;

	const InputManager* input;
	FrameRateController* frameRateController;

private:
	SOCKET mSocket;
	SOCKADDR_IN mServerAddress;

	int mFlags;
};