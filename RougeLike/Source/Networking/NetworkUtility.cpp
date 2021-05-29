#include "pch.h"
#include "NetworkUtility.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

namespace Networking
{
	hostent* getHostByAddress(const BasicString& ipAddress)
	{
		in_addr addr = ipAddressToInAddr(ipAddress);
		
		hostent* data = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
		if (!data)
		{
			DebugPrint(Warning, "No host data at ip address '%s', error: %d\n", ipAddress.c_str(), WSAGetLastError());
		}

		return data;
	}


	in_addr ipAddressToInAddr(const BasicString& ipAddress)
	{
		in_addr address;
		inet_pton(AF_INET, ipAddress.c_str(), &address);
		return address;
	}


	void displayInfo()
	{
		DebugPrint(Log, "Host name: '%s'\n", getHostName().c_str());
		DebugPrint(Log, "Host Address: '%s'\n", getHostIPAddress().c_str());
	}



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
		if (!hostData)
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