#include "pch.h"
#include "NetworkDebugging.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

namespace Networking
{
	BasicString getHostName()
	{
		BasicString hostName("", 255);
		int result = gethostname(hostName.buffer(), 255);
		if (result)
			DebugPrint(Log, "getHostName error code: %d\n", WSAGetLastError());

		return hostName;
	}


	BasicString getHostIPAddress()
	{
		hostent *hostData = gethostbyname(getHostName().c_str());
		if(!hostData)
			DebugPrint(Log, "gethostbyname error code: %d\n", WSAGetLastError());

		return getDataIPAddress(hostData);
	}


	BasicString getHostFromIP()
	{
		BasicString ipAddress = getHostIPAddress();

		hostent *hostData1 = gethostbyname(getHostName().c_str());
		in_addr* address = (in_addr*)hostData1->h_addr_list[0];


		int size = ipAddress.length();
		hostent *hostData = gethostbyaddr((char*)address, size, AF_INET);

		if (!hostData)
			DebugPrint(Log, "gethostbyaddr error code: %d\n", WSAGetLastError());

		BasicString ip = getDataIPAddress(hostData);
		//BasicString hostName = getDataHostName(hostData);

		int a = 4;
		return BasicString();
	}



	BasicString getDataIPAddress(const hostent* data)
	{
		if (data)
		{
			char* IP = inet_ntoa(*((struct in_addr*) data->h_addr_list[0]));
			return BasicString(IP);
		}
		else
		{
			DebugPrint(Warning, "hostent data = nullptr, cannot get IP address\n");
			return BasicString();
		}
	}
}