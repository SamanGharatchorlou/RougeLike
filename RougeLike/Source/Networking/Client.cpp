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
			BasicString bufferName("", 255);

			gethostname(bufferName.buffer(), 255);

			//if (!GetComputerName(bufferName.c_str(), 255))
			//	printError(TEXT("GetComputerName"));
			//_tprintf(TEXT("\nComputer name:      %s"), infoBuf);


			BasicString hostName("", 255);
			printf("enter host name: ");
			std::cin.get(hostName.buffer(), hostName.bufferLength());
			std::cin.ignore();

			hostName = "DESKTOP-6152CHS";

			hostent *host_entry = gethostbyname(hostName.c_str());

			if(!host_entry)
				DebugPrint(Warning, "not host entry value, error: %d\n", WSAGetLastError());

			mServerAddress.sin_family = AF_INET;
			mServerAddress.sin_port = htons(9999);
			mServerAddress.sin_addr = *(in_addr*)*host_entry->h_addr_list;

#if DEBUG_CHECK
			char* localIP;
			localIP = inet_ntoa(*(in_addr*)*host_entry->h_addr_list);
			DebugPrint(Log, "connecting to host at ip: %s\n", localIP);
#endif
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