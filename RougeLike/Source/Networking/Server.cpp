#include "pch.h"
#include "Server.h"


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
	}

	char hostName[255];
	gethostname(hostName, 255);

	hostent *host_entry = gethostbyname(hostName);

	char* localIP;
	localIP = inet_ntoa(*(in_addr*)*host_entry->h_addr_list);

	int addrFamily = AF_INET;
	int type = SOCK_DGRAM;
	int protocol = IPPROTO_UDP;
	mSocket = socket(addrFamily, type, protocol);

	if (mSocket == INVALID_SOCKET)
	{
		DebugPrint(Warning, "socket failed %d\n", WSAGetLastError());
	}

	SOCKADDR_IN localAddress;
	localAddress.sin_family = AF_INET;
	localAddress.sin_port = htons(9999);
	localAddress.sin_addr.s_addr = INADDR_ANY;

	int result = bind(mSocket, (SOCKADDR*)&localAddress, sizeof(localAddress));

	if (result)
	{
		DebugPrint(Warning, "bind failed %d\n", WSAGetLastError());
	}
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