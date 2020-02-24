#pragma once

#include "Characters/Attributes/Health.h"

enum class Event
{
	None,
	EnemyDead,
	SetHealth,
	UpdateScore,
};


struct EventData
{
	virtual ~EventData() = 0;
};


struct EventPacket
{
	EventPacket(const Event theEvent, const EventData* dataPtr) : event(theEvent), data(dataPtr) { }
	~EventPacket() { }

	void free();

	const Event event;
	const EventData* data;
};


// testing
struct PrintEvent : public EventData
{
	PrintEvent(std::string printString) : str(printString) { }
	~PrintEvent() { }
	std::string str;
};


struct EnemyDeadEvent : public EventData
{
	EnemyDeadEvent(int score, int exp) : mScore(score), mExp(exp) { }
	~EnemyDeadEvent() { }
	const int mScore;
	const int mExp;
};


struct SetHealthBarEvent : public EventData
{
	SetHealthBarEvent(Health hp) : health(hp) { }
	~SetHealthBarEvent() { }
	const Health health;
};


struct UpdateScoreEvent : EventData
{
	UpdateScoreEvent(int score) : mScore(score) { }
	~UpdateScoreEvent() { }
	const int mScore;
};


struct Observer
{
	virtual ~Observer() = default;
	virtual void handleEvent(const Event event, EventData& data) = 0;
	virtual void handleEvent(const EventPacket) = 0;
};