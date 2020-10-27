#include "pch.h"
#include "Client.h"


Client::Client() : mSocket(INVALID_SOCKET), mFlags(0) { }


void Client::open()
{
	WORD winsockVersion = 0x202;
	WSADATA winsockData;

	if (WSAStartup(winsockVersion, &winsockData) == 0)
	{
		int addrFamily = AF_INET;
		int type = SOCK_DGRAM;
		int protocol = IPPROTO_UDP;
		mSocket = socket(addrFamily, type, protocol);

		if (mSocket != INVALID_SOCKET)
		{
			mServerAddress.sin_family = AF_INET;
			mServerAddress.sin_port = htons(9999);
			mServerAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		}
		else
		{
			DebugPrint(Warning, "socket failed %d\n", WSAGetLastError());
		}
	}
	else
	{
		DebugPrint(Warning, "WSAStartup failed %d\n", WSAGetLastError());
	}
}


void Client::close()
{
	mSocket = INVALID_SOCKET;
	WSACleanup();
}


void Client::sendMessage(const BasicString& message)
{

	int result = sendto(mSocket, message.c_str(), message.length() + 1, mFlags, (SOCKADDR*)&mServerAddress, sizeof(mServerAddress)) == SOCKET_ERROR;

	if (result)
	{
		DebugPrint(Warning, "sendto failed %d\n", WSAGetLastError());
	}
}