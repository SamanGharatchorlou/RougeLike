#include "pch.h"
#include "Server.h"

#include "Debug/NetworkDebugging.h"


Server::Server() : mSocket(INVALID_SOCKET), mFlags(0)
{

}


void Server::open()
{
	WORD winsockVersion = 0x202;
	WSADATA winsockData;

	if (WSAStartup(winsockVersion, &winsockData))
	{
		DebugPrint(Warning, "WSAStartup failed %d\n", WSAGetLastError());
		return;
	}

	int addrFamily = AF_INET;
	int type = SOCK_DGRAM;
	int protocol = IPPROTO_UDP;
	mSocket = socket(addrFamily, type, protocol);

	if (mSocket == INVALID_SOCKET)
	{
		DebugPrint(Warning, "socket failed %d\n", WSAGetLastError());
		return;
	}

	SOCKADDR_IN localAddress;
	localAddress.sin_family = AF_INET;
	localAddress.sin_port = htons(9999);
	localAddress.sin_addr.s_addr = INADDR_ANY;

	int result = bind(mSocket, (SOCKADDR*)&localAddress, sizeof(localAddress));

	if (result)
	{
		DebugPrint(Warning, "bind failed %d\n", WSAGetLastError());
		return;
	}

#if DEBUG_CHECK
	BasicString hostName = Networking::getHostName();
	BasicString hostIP = Networking::getHostIPAddress();
	DebugPrint(Log, "Host Name: '%s'\n", hostName.c_str());
	DebugPrint(Log, "Host IP  : '%s'\n", hostIP.c_str());
	DebugPrint(Log, "Server opened\n");

	BasicString string = Networking::getHostFromIP();
#endif
	
}


void Server::close()
{
	mSocket = INVALID_SOCKET;
	WSACleanup();
}


void Server::receiveMessage(BasicString& outMessage, BasicString* senderInfo)
{
	constexpr int SOCKET_BUFFER_SIZE = 1024;
	char buffer[SOCKET_BUFFER_SIZE];

	SOCKADDR_IN from;
	int fromSize = sizeof(from);

 	int bytesReceived = recvfrom(mSocket, buffer, SOCKET_BUFFER_SIZE, mFlags, (SOCKADDR*)&from, &fromSize);

	if (bytesReceived == SOCKET_ERROR)
	{
		printf("recvfrom returned SOCKET_ERROR %d\n", WSAGetLastError());
	}
	else
	{
		if (senderInfo)
		{
			sprintf(senderInfo->buffer(), "%d.%d.%d.%d:%d",
				from.sin_addr.S_un.S_un_b.s_b1,
				from.sin_addr.S_un.S_un_b.s_b2,
				from.sin_addr.S_un.S_un_b.s_b3,
				from.sin_addr.S_un.S_un_b.s_b4,
				from.sin_port);
		}

		outMessage.set(buffer);
	}
}