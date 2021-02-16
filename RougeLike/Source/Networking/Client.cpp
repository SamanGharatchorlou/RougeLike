#include "pch.h"
#include "Client.h"
#include "Debug/NetworkDebugging.h"

#include "NetworkUtility.h"

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
			BasicString hostName("", 255);
			printf("entering host name 'DESKTOP-6152CHS' for you\n");
			//hostName.getInput("enter host name: ");
			hostName = "DESKTOP-6152CHS";

			hostent* hostData = gethostbyname(hostName.c_str());

			if (!hostData)
			{
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

#if DEBUG_CHECK
				BasicString hostIPAddress = Networking::getDataIPAddress(hostData);
				DebugPrint(Log, "Connecting to host at ip: %s\n", hostIPAddress.c_str());
#endif
			}
		}
		else
		{
			DebugPrint(Warning, "Socket failed %d\n", WSAGetLastError());
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