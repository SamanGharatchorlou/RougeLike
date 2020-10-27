#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

class Client
{
public:
	Client();

	void open();

	void sendMessage(const BasicString& message);

	void close();


private:
	SOCKET mSocket;
	SOCKADDR_IN mServerAddress;

	int mFlags;
};