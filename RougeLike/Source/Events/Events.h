#pragma once

#include "Characters/Attributes/Health.h"


enum class Event
{
	None,
	EnemyDead,
	SetHealth,
	UpdateScore,
	UpdateTextBox,
	Trauma,
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


// TODO: make the EventData base class private?
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


struct UpdateTextBoxEvent : public EventData
{
	UpdateTextBoxEvent(std::string id, int value) : mId(id), mValue(value) { }
	~UpdateTextBoxEvent() { }

	const std::string mId;
	const int mValue;
};


struct TraumaEvent : public EventData
{
	TraumaEvent(int trauma) : mTrauma(trauma) { }

	const int mTrauma;
};