#pragma once

enum class Event
{
	None,
	EnemyDead
};


struct EventData
{
	EventData(Event type) : mType(type) { }
	virtual ~EventData() { };

	Event mType = Event::None;
};


class EnemyDeadData : public EventData
{
public:
	EnemyDeadData() : EventData(Event::EnemyDead) { }
	~EnemyDeadData() { }
	int score;
};


struct Observer
{
	virtual ~Observer() = default;
	virtual void handleEvent(Event event, EventData& data) = 0;
};