#pragma once

#include <string>
#include "Utilities/Rect.h"

#include "Objects/Attributes/Health.h"
#include "Objects/Attributes/Armor.h"

class Enemy;
class TargePositionAttackAbility;
class Texture;

enum class Event
{
	None,

	SetHealth,
	SetArmor,
	EnemyDead,
	EnemyHit,

	IncrementMapLevel,

	SetGameVolume,
	SetMusicVolume,

	UpdateLoadingBar,

	UpdateTextBox,
	MoveUIElement,
	ChangeUIElementSize,
	SetTextColour,
	SetUIRect,
	SetUIBar,

	Trauma,

	UpdateAIPathMap,
	UpdateAICostMap,

	ActivateAbilityOn,

	Render
};


struct EventData
{
	virtual ~EventData() = 0;

	void setEventType(Event event) { eventType = event; }
	Event eventType = Event::None;
};


struct EventPacket
{
	EventPacket(EventData* dataPtr) : data(dataPtr) { }
	~EventPacket() { }

	void free()
	{
		delete data;
		data = nullptr;
	}

	EventData* data;
};


struct RenderEvent : public EventData
{
	RenderEvent(Texture* texture, RectF rect, int renderLayer) : mTexture(texture), mRect(rect), mRenderLayer(renderLayer) { eventType = Event::Render; }
	Texture* mTexture;
	RectF mRect;
	int mRenderLayer;
};


struct SetTextColourEvent : public EventData
{
	SetTextColourEvent(const std::string& id, SDL_Color colour) : mId(id), mColour(colour) { eventType = Event::SetTextColour; }
	SDL_Color mColour;
	const std::string mId;
};


struct ActivateAreaAttack : public EventData
{
	ActivateAreaAttack(const TargePositionAttackAbility* ability) : mAbility(ability) { eventType = Event::ActivateAbilityOn; }
	~ActivateAreaAttack() { }
	const TargePositionAttackAbility* mAbility;
};


struct UpdateLoadingBarEvent : public EventData
{
	UpdateLoadingBarEvent() { eventType = Event::UpdateLoadingBar; }
	~UpdateLoadingBarEvent() { }
};


struct EnemyDeadEvent : public EventData
{
	EnemyDeadEvent(const Enemy* enemy, const float score, const float exp) : mEnemy(enemy), mScore(score), mExp(exp) { eventType = Event::EnemyDead; }
	~EnemyDeadEvent() { }
	const Enemy* mEnemy;
	const float mScore;
	const float mExp;
};


struct SetUIBarEvent : public EventData
{
	SetUIBarEvent(const std::string bar, const std::string barContainer, const float percentage) : mBar(bar), mBarContainer(barContainer), mPercentage(percentage) { }
	~SetUIBarEvent() { };

	const std::string mBar;
	const std::string mBarContainer;
	const float mPercentage;
};


struct SetHealthBarEvent : public EventData
{
	SetHealthBarEvent(Health hp) : health(hp) { eventType = Event::SetHealth; }
	~SetHealthBarEvent() { }
	const Health health;
};


struct SetArmorBarEvent : public EventData
{
	SetArmorBarEvent(Armor armor) : mArmor(armor) { eventType = Event::SetArmor; }
	~SetArmorBarEvent() { }
	const Armor mArmor;
};


struct IncrementLevelEvent : public EventData
{
	IncrementLevelEvent() { eventType = Event::IncrementMapLevel; };
	~IncrementLevelEvent() { }
};


struct UpdateTextBoxEvent : public EventData
{
	UpdateTextBoxEvent(const std::string& id, int value) : mId(id), mValue(value) { eventType = Event::UpdateTextBox; }
	~UpdateTextBoxEvent() { }

	const std::string mId;
	const int mValue;
};


struct TraumaEvent : public EventData
{
	TraumaEvent(int trauma) : mTrauma(trauma) { eventType = Event::Trauma; }
	~TraumaEvent() { }

	const int mTrauma;
};


struct UpdateAIPathMapEvent : public EventData
{
	UpdateAIPathMapEvent() { eventType = Event::UpdateAIPathMap; };
	~UpdateAIPathMapEvent() { }
};


struct UpdateAICostMapEvent : public EventData
{
	UpdateAICostMapEvent() { eventType = Event::UpdateAICostMap; };
	~UpdateAICostMapEvent() { }
};


struct EditUIRectEvent : public EventData
{
	EditUIRectEvent(const std::string& id, VectorF change) : mId(id), mChange(change) { };
	~EditUIRectEvent() { };

	const std::string mId;
	const VectorF mChange;
};

struct SetUIRectEvent : public EventData
{
	SetUIRectEvent(const std::string& id, RectF rect) : mId(id), mRect(rect) { eventType = Event::SetUIRect; }
	~SetUIRectEvent() { }

	const std::string mId;
	const RectF mRect;
};

struct SetVolumeEvent : public EventData
{
	SetVolumeEvent(float volume) : mVolume(volume) { } 

	const float mVolume;
};