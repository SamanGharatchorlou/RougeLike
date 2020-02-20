#pragma once

enum class Event
{
	None,
	EnemyDead
};


struct EventData
{
	virtual ~EventData() { };
};


struct EnemyDeadData : public EventData
{
	int score;
};

struct PlayerTakenDamage : public EventData
{
	int damage;
};


struct Observer
{
	virtual ~Observer() = default;
	virtual void handleEvent(Event event, EventData& data) = 0;
};