#pragma once
#include "Observer.h"

class Dispatcher
{
public:
	void addObserver(Observer* observer);
	void removeObserver(Observer* observer);

	void notify(Event type, EventData event);

private:
	std::vector<Observer*> observers;
};