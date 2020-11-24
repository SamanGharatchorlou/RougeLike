#include "pch.h"
#include "Client.h"
#include "Debug/NetworkDebugging.h"

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
			hostName.getInput("enter host name: ");
			hostName = "LAPTOP-45AOTVEA";

			hostent *host_entry = gethostbyname(hostName.c_str());

			if (!host_entry)
			{
				DebugPrint(Warning, "no host data for host name '%s', error: %d\n", hostName.c_str(), WSAGetLastError());

				BasicString ipAddress("", 255);
				ipAddress.getInput("Enter host ip address: ");

				int size = ipAddress.length();
				hostent *hostData = gethostbyaddr(ipAddress.c_str(), size, AF_INET);

				if (hostData)
				{
					BasicString hostIPAddress = Networking::getDataIPAddress(host_entry);
					DebugPrint(Log, "connecting to host at ip: %s\n", hostIPAddress.c_str());
				}
				else
				{
					DebugPrint(Warning, "no host data at ip address '%s', error: %d\n", ipAddress.c_str(), WSAGetLastError());
				}
			}
			else
			{
				mServerAddress.sin_family = AF_INET;
				mServerAddress.sin_port = htons(9999);
				mServerAddress.sin_addr = *(in_addr*)*host_entry->h_addr_list;

#if DEBUG_CHECK
				BasicString hostIPAddress = Networking::getDataIPAddress(host_entry);
				DebugPrint(Log, "connecting to host at ip: %s\n", hostIPAddress.c_str());
#endif
			}



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