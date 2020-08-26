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

	HealthChanged,

	SetGameVolume,
	SetMusicVolume,

	UpdateLoadingBar,
	UpdateTextBox,
	SetTextColour,
	SetUIBar,
	SetUISlider,
	OpenPopup,

	Trauma,

	UpdateAIPathMap,
	UpdateAICostMap,
	LevelUpdated,

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


struct OpenPopupEvent : public EventData
{
	OpenPopupEvent(const BasicString& infoID) : EventData(Event::OpenPopup), mInfoID(infoID) { }

	const BasicString mInfoID;
};


struct LevelUpdatedEvent : public EventData
{
	enum MapStatus
	{
		Added,
		Removed
	};

	LevelUpdatedEvent(MapStatus status) : EventData(Event::LevelUpdated), mStatus(status) { }
	MapStatus mStatus;
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


struct HealthChangedEvent : public EventData
{
	HealthChangedEvent() : EventData(Event::HealthChanged) { }
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
	EnemyDeadEvent(const Enemy* enemy, int score, int exp) : EventData(Event::EnemyDead), mEnemy(enemy), mScore(score), mExp(exp) { }

	const Enemy* mEnemy;
	const int mScore;
	const int mExp;
};


struct SetUIBarEvent : public EventData
{
	SetUIBarEvent(const BasicString bar, const BasicString barContainer, const float percentage) : EventData(Event::SetUIBar),
		mBar(bar), mBarContainer(barContainer), mPercentage(percentage) { }

	const BasicString mBar;
	const BasicString mBarContainer;
	const float mPercentage;
};


struct UpdateTextBoxEvent : public EventData
{
	UpdateTextBoxEvent(const BasicString& id, const BasicString& text) : EventData(Event::UpdateTextBox), mId(id), mText(text) { }

	const BasicString mId;
	const BasicString mText;
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