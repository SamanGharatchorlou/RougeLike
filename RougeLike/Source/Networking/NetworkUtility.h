#pragma once

struct hostent;
struct in_addr;

namespace Networking
{
	hostent* getHostByAddress(const BasicString& ipAddress);
	in_addr ipAddressToInAddr(const BasicString& ipAddress);
}