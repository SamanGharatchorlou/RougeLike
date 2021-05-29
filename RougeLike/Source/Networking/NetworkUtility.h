#pragma once

struct hostent;
struct in_addr;

namespace Networking
{
	hostent* getHostByAddress(const BasicString& ipAddress);
	in_addr ipAddressToInAddr(const BasicString& ipAddress);

	BasicString getHostName();
	BasicString getHostIPAddress();

	BasicString getHostFromIP(); // broken?

	BasicString getDataIPAddress(const hostent* data);

	void displayInfo();
}
