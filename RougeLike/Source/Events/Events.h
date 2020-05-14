#pragma once

#include "Objects/Attributes/Health.h"

class Enemy;

enum class Event
{
	None,

	SetHealth,
	EnemyDead,
	EnemyHit,

	IncrementMapLevel,

	UpdateTextBox,
#if UI_EDITOR
	MoveUIElement,
	ChangeUIElementSize,
#endif

	Trauma,

	UpdateAIPathMap
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



struct EnemyDeadEvent : public EventData
{
	EnemyDeadEvent(const Enemy* enemy, const float score, const float exp) : mEnemy(enemy), mScore(score), mExp(exp) { }
	~EnemyDeadEvent() { }
	const Enemy* mEnemy;
	const float mScore;
	const float mExp;
};


struct SetHealthBarEvent : public EventData
{
	SetHealthBarEvent(Health hp) : health(hp) { }
	~SetHealthBarEvent() { }
	const Health health;
};


struct IncrementLevelEvent : public EventData
{
	IncrementLevelEvent() { };
	~IncrementLevelEvent() { }
};


struct UpdateTextBoxEvent : public EventData
{
	UpdateTextBoxEvent(const std::string& id, int value) : mId(id), mValue(value) { }
	~UpdateTextBoxEvent() { }

	const std::string mId;
	const int mValue;
};


struct TraumaEvent : public EventData
{
	TraumaEvent(int trauma) : mTrauma(trauma) { }

	const int mTrauma;
};


struct UpdateAIPathMapEvent : public EventData
{
	UpdateAIPathMapEvent() { };
	~UpdateAIPathMapEvent() { }
};


#if UI_EDITOR
struct EditUIRectEvent : public EventData
{
	EditUIRectEvent(const std::string& id, VectorF change) : mId(id), mChange(change) { };
	~EditUIRectEvent() { };

	const std::string mId;
	VectorF mChange;
};
#endif