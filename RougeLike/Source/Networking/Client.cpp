#include "pch.h"
#include "Client.h"

#include "NetworkUtility.h"

Client::Client() : mSocket(INVALID_SOCKET), mFlags(0) { }


void Client::open()
{

	WORD winsockVersion = 0x202;
	WSADATA winsockData;

	if (WSAStartup(winsockVersion, &winsockData) == 0)
	{
#if NETWORK_TESTING
		Networking::displayInfo();
#endif

		int addrFamily = AF_INET;
		int type = SOCK_DGRAM;
		int protocol = IPPROTO_UDP;
		mSocket = socket(addrFamily, type, protocol);

		if (mSocket != INVALID_SOCKET)
		{
			BasicString hostName("", 255);
			printf("entering host name 'DESKTOP-6152CHS' for you\n");
			//hostName.getInput("enter host name: ");
			hostName = "DESKTOP-6152CHS";

			hostent* hostData = gethostbyname(hostName.c_str());

			if (!hostData)
			{
				// Warning: Error code 11001 may be triggered here, removing firewalls fixes this issue. A very legit fix...
				DebugPrint(Warning, "No host data for host name '%s', error: %d\n", hostName.c_str(), WSAGetLastError());

				BasicString ipAddress("", 255);
				ipAddress.getInput("Enter host ip address: ");

				hostData = Networking::getHostByAddress(ipAddress);

				if (!hostData)
					DebugPrint(Warning, "Unable to connect to a host, clien open failed\n");
			}

			if(hostData)
			{
				mServerAddress.sin_family = AF_INET;
				mServerAddress.sin_port = htons(9999);
				mServerAddress.sin_addr = *(in_addr*)*hostData->h_addr_list;
#if NETWORK_TESTING
				BasicString hostIPAddress = Networking::getDataIPAddress(hostData);
				DebugPrint(Log, "Connected to host at ip: %s\n", hostIPAddress.c_str());
#endif
			}
		}
		else
		{
			DebugPrint(Warning, "Socket failed %d\n", WSAGetLastError());
			return;
		}
	}
	else
	{
		DebugPrint(Warning, "WSAStartup failed %d\n", WSAGetLastError());
		return;
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

	sending = false;
}

void Client::sendMessage(NetworkInputData& data)
{
	int result = sendto(mSocket, data.buffer, data.size, mFlags, (SOCKADDR*)&mServerAddress, sizeof(mServerAddress)) == SOCKET_ERROR;

	if (result)
	{
		DebugPrint(Warning, "sendto failed %d\n", WSAGetLastError());
	}

	sending = false;
}

void Client::receiveMessage(BasicString& outMessage, BasicString* senderInfo)
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

		sending = true;
	}
}