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
	virtual ~EventData() { };
};


struct EnemyDeadEvent : public EventData
{
	EnemyDeadEvent(int score) : mScore(score) { }
	const int mScore;
};


struct SetHealthBarEvent : public EventData
{
	SetHealthBarEvent(Health maxHp, Health hp) : maxHealth(maxHp), health(hp) { }
	const Health maxHealth;
	const Health health;
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