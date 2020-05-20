#pragma once

#include "Events.h"


struct Observer
{
	virtual ~Observer() = default;
	virtual void handleEvent(EventData& data) = 0;
};


