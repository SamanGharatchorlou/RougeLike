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

	//void getClient

	void receiveMessage(BasicString& outMessage, BasicString* senderInfo);
	void sendMessage(const BasicString& message);

	Queue<BasicString> mInputs;

	bool sending = false;

private:
	SOCKET mSocket;
	int mFlags;


	SOCKADDR_IN mClientAddress;
};
