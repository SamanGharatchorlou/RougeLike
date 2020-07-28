#pragma once

#include "Events/Events.h"
#include "Objects/Effects/EffectTypes/Effect.h"

#include "Animations/Animator.h"
#include "Collisions/Collider.h"
#include "Objects/Abilities/Cooldown.h"

#include "Objects/Properties/PropertyBag.h"


class Actor;
class Map;
class Player;
class EffectPool;


enum class AbilityType
{
	None,
	Heal
};


inline void operator >>(AbilityType a, std::string& str)
{
	if (a == AbilityType::Heal)
		str = "Heal";
}


class Ability
{
public:
	enum class TargetType
	{
		Self,
		Actor,
		Position,
		AttackArea,
	};

	enum State
	{
		None,
		Idle,
		Selected,
		Activate,
		Running,
		Finished
	};


public:
	Ability() : mState(None) { }
	virtual ~Ability() { }

	void fillAbilityValues(const ValueBag& values);	
	virtual void init(Animator animator, Actor* caster);

	virtual void fillValues(const ValueBag& values) { };
	virtual void fastUpdate(float dt) = 0;
	virtual void slowUpdate(float dt) = 0;
	virtual void render() = 0;
	virtual void exit();

	virtual TargetType targetType() const = 0;
	virtual AbilityType type() const { return AbilityType::None; } // = 0;

	Actor* caster() const { return mCaster; }

	void setName(const std::string& name) { mName = name; }
	std::string name() const { return mName; }

	void setState(State state)	{ mState = state; }
	State state() const			{ return mState; }

	void pushEvent(EventPacket event) { mEvents.push(event); }
	bool hasEvent() const { return mEvents.size() > 0; }
	EventPacket popEvent();

	Cooldown& cooldown() { return mCooldown; }

protected:
	std::string mName;

	State mState;
	Animator mAnimator;
	std::queue<EventPacket> mEvents; // TODO: replace with local dispatcher

	Actor* mCaster;
	RectF mRect;
	float mMaxDimention;

	Cooldown mCooldown;
};


class RangedAbility : public Ability
{
public:
	void fillRangedAbilityValues(const ValueBag& values);
	void setRangeCircle(Texture* rangeCircle) { mRangeCircle = rangeCircle; }

	void renderRangeCircle();

	bool isValidTarget(VectorF target, const Map* map);

	RectF effectArea() const { return mRect; }
	Collider collider();


protected:
	Map* mMap;

	float mRange;
	Texture* mRangeCircle;
};


class TargetSelfAbility : public Ability
{
public:
	virtual void activate(EffectPool* pool) = 0;

	TargetType targetType() const override { return TargetType::Self; }

	virtual void render() override;
};


class TargetActorAbility : public RangedAbility
{
public:
	virtual void activateOn(Actor* target, EffectPool* pool) = 0;

	TargetType targetType() const override { return TargetType::Actor; }

	void render() override { }
};


class TargetPositionAbility : public RangedAbility
{
public:
	virtual void activateAt(VectorF position, EffectPool* pool) = 0;

	TargetType targetType() const override { return TargetType::Position; }

	virtual void render() override;
};


class TargePositionAttackAbility : public RangedAbility
{
public:
	virtual void activateAt(VectorF position, EffectPool* pool) = 0;
	virtual void activateOn(Actor* target, EffectPool* pool) = 0;

	TargetType targetType() const override { return TargetType::AttackArea; }

	virtual void render() override;

	void sendActivateOnRequest();
};
