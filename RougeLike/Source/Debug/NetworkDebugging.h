#pragma once

struct hostent;

namespace Networking
{

	BasicString getHostName();
	BasicString getHostIPAddress();

	BasicString getHostFromIP();

	BasicString getDataIPAddress(const hostent* data);
	//BasicString getDataHostName(const hostent* data);
}
