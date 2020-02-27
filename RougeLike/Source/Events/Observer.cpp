#include "pch.h"
#include "Observer.h"


EventData::~EventData() { /*printf("destroy event data\n");*/ }


void EventPacket::free()
{
	delete data;
	data = nullptr;
}