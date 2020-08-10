#pragma once

#include <queue>
#include "Events.h"


class LocalDispatcher
{
public:
	EventPacket pop()
	{
		EventPacket event = mEvents.front();
		mEvents.pop();
		return event;
	}
	void push(const EventPacket event) { mEvents.push(event); }
	bool hasEvent() const { return mEvents.size() > 0; }
 
	void clear() { while (hasEvent()) pop(); }


private:
	std::queue<EventPacket> mEvents;
};