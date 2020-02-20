#pragma once

enum class Event
{
	None,
	EnemyDead,
	UpdateScore,
};


struct EventData
{
	virtual ~EventData() { };
};


struct EnemyDeadEvent : public EventData
{
	EnemyDeadEvent(int score) : mScore(score) { }
	const int mScore;
};

struct PlayerTakenDamageEvent : public EventData
{
	PlayerTakenDamageEvent(int damage) : mDamage(damage) { }
	const int mDamage;
};

struct UpdateScoreEvent : EventData
{
	UpdateScoreEvent(int score) : mScore(score) { }
	const int mScore;
};


struct Observer
{
	virtual ~Observer() = default;
	virtual void handleEvent(Event event, EventData& data) = 0;
};