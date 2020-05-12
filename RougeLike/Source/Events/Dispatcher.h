#pragma once

#include "Observer.h"

class Observer;

class Dispatcher
{
public:
	void addObserver(Observer* observer);
	void removeObserver(Observer* observer);

	void notify(const Event type, EventData& event);


private:
	std::vector<Observer*> observers;
};