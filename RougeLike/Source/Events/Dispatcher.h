#pragma once

#include "Observer.h"

struct Observer;

class Dispatcher
{
public:
	void addObserver(Observer* observer);
	void removeObserver(Observer* observer);

	void notify(EventData& event);


private:
	std::vector<Observer*> observers;
};