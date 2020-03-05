#pragma once

#include "Events.h"


struct Observer
{
	virtual ~Observer() = default;
	virtual void handleEvent(const Event event, EventData& data) = 0;
};


