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
}