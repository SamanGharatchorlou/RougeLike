#pragma once

#include <string>
#include "Utilities/Rect.h"

#include "Objects/Properties/Attributes/Armor.h"
#include "Objects/Properties/Attributes/Health.h"

class Enemy;
class TargePositionAttackAbility;
class Texture;


enum class Event
{
	None,

	EnemyDead,
	EnemyHit,

	IncrementMapLevel,

	SetGameVolume,
	SetMusicVolume,

	UpdateLoadingBar,
	UpdateTextBox,
	SetTextColour,
	SetUIBar,
	SetUISlider,

	Trauma,

	UpdateAIPathMap,
	UpdateAICostMap,

	ActivateAbilityOn,

	Render,

#if UI_EDITOR
	MoveUIElement,
	ChangeUIElementSize,
#endif
};


struct EventData
{
	EventData() { }
	EventData(Event type) : eventType(type) {}
	virtual ~EventData() { }

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
	RenderEvent(Texture* texture, RectF rect, int renderLayer) : EventData(Event::Render), mTexture(texture), mRect(rect), mRenderLayer(renderLayer) { }

	Texture* mTexture;
	RectF mRect;
	int mRenderLayer;
};


struct SetTextColourEvent : public EventData
{
	SetTextColourEvent(const BasicString& id, SDL_Color colour) : EventData(Event::SetTextColour), mId(id), mColour(colour) { }

	SDL_Color mColour;
	const BasicString mId;
};


struct SetUISlider : public EventData
{
	SetUISlider(const BasicString& id, float value) : EventData(Event::SetUISlider), mId(id), mValue(value) { }

	const BasicString mId;
	float mValue;
};


struct ActivateAreaAttack : public EventData
{
	ActivateAreaAttack(const TargePositionAttackAbility* ability) : EventData(Event::ActivateAbilityOn), mAbility(ability) { }

	const TargePositionAttackAbility* mAbility;
};


struct UpdateLoadingBarEvent : public EventData
{
	UpdateLoadingBarEvent() : EventData(Event::UpdateLoadingBar) { }
};


struct EnemyDeadEvent : public EventData
{
	EnemyDeadEvent(const Enemy* enemy, const float score, const float exp) : EventData(Event::EnemyDead), mEnemy(enemy), mScore(score), mExp(exp) { }

	const Enemy* mEnemy;
	const float mScore;
	const float mExp;
};


struct SetUIBarEvent : public EventData
{
	SetUIBarEvent(const BasicString bar, const BasicString barContainer, const float percentage) : EventData(Event::SetUIBar),
		mBar(bar), mBarContainer(barContainer), mPercentage(percentage) { }

	const BasicString mBar;
	const BasicString mBarContainer;
	const float mPercentage;
};


struct IncrementLevelEvent : public EventData
{
	IncrementLevelEvent() : EventData(Event::IncrementMapLevel) { };
};


struct UpdateTextBoxEvent : public EventData
{
	UpdateTextBoxEvent(const BasicString& id, int value) : EventData(Event::UpdateTextBox), mId(id), mValue(value) { }

	const BasicString mId;
	const int mValue;
};


struct TraumaEvent : public EventData
{
	TraumaEvent(int trauma) : EventData(Event::Trauma), mTrauma(trauma) { }

	const int mTrauma;
};


struct UpdateAIPathMapEvent : public EventData
{
	UpdateAIPathMapEvent() : EventData(Event::UpdateAIPathMap) { };
};


struct UpdateAICostMapEvent : public EventData
{
	UpdateAICostMapEvent() : EventData(Event::UpdateAICostMap) { };
};


struct SetVolumeEvent : public EventData
{
	SetVolumeEvent(float volume) : mVolume(volume) { } 

	const float mVolume;
};



#if UI_EDITOR
struct EditUIRectEvent : public EventData
{
	EditUIRectEvent(const BasicString& id, VectorF change) : mId(id), mChange(change) { };
	~EditUIRectEvent() { };

	const BasicString mId;
	const VectorF mChange;
};
#endif