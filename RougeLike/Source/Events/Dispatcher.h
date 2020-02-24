#pragma once
#include "Observer.h"

class Dispatcher
{
public:
	void addObserver(Observer* observer);
	void removeObserver(Observer* observer);

	void notify(const Event type, EventData& event);
	void notify(const EventPacket eventPacket);


private:
	std::vector<Observer*> observers;
};