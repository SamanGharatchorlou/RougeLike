#pragma once

#include <iostream>

#include <winsock2.h>
#include <Windows.h>

#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

#include "NetworkData.h"


class Server
{
public:
	Server();

	void open();
	void close();

	//void getClient

	void receiveMessage(NetworkData& outMessage, BasicString* senderInfo);
	void receiveMessage(BasicString& outMessage, BasicString* senderInfo);
	void sendMessage(const BasicString& message);

	Queue<NetworkData> mData;

	bool sending = false;

private:
	SOCKET mSocket;
	int mFlags;


	SOCKADDR_IN mClientAddress;
};
