#include "pch.h"
#include "Dispatcher.h"


void Dispatcher::addObserver(Observer* observer) 
{ 
	observers.push_back(observer); 
}


void Dispatcher::removeObserver(Observer* observer) 
{
	for (std::vector<Observer*>::iterator iter = observers.begin(); iter != observers.end(); iter++)
	{
		if (*iter == observer)
		{
			observers.erase(iter);
		}
	}
}


void Dispatcher::notify(EventData& data)
{
	for (unsigned int i = 0; i < observers.size(); i++)
	{
		observers[i]->handleEvent(data);
	}
}
