#pragma once

#include "Characters/Attributes/Health.h"

enum class Event
{
	None,
	EnemyDead,
	SetHealth,
	UpdateScore,
	UpdateUIValue
};


struct EventData
{
	virtual ~EventData() = 0;
};


struct EventPacket
{
	EventPacket(const Event theEvent, EventData* dataPtr) : event(theEvent), data(dataPtr) { }
	~EventPacket() { }

	void free();

	const Event event;
	EventData* data;
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

// TODO: replace with update ui value
struct UpdateScoreEvent : EventData
{
	UpdateScoreEvent(int score) : mScore(score) { }
	~UpdateScoreEvent() { }
	const int mScore;
};


struct UpdateUIValueEvent : EventData
{
	UpdateUIValueEvent(std::string id, int value) : mId(id), mValue(value)  { }
	~UpdateUIValueEvent() { }

	const std::string mId;
	const int mValue;
};

struct Observer
{
	virtual ~Observer() = default;
	virtual void handleEvent(const Event event, EventData& data) = 0;
};