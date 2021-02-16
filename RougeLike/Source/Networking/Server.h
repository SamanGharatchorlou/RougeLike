#pragma once

#include <iostream>

#include <winsock2.h>
#include <Windows.h>

#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")


class Server
{
public:
	Server();

	void open();
	void close();

	void receiveMessage(BasicString& outMessage, BasicString* senderInfo);


	Queue<BasicString> mInputs;

private:
	SOCKET mSocket;
	int mFlags;
};
