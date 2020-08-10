#pragma once

#include "AbilityStates.h"
#include "Events/LocalDispatcher.h"
#include "Animations/Animator.h"
#include "Objects/Abilities/Cooldown.h"


class Actor;
class PropertyBag;


class Ability
{
public:
	Ability() : mState(AbilityState::None), mCaster(nullptr), mMaxDimention(0.0f) { }
	virtual ~Ability() { }

	virtual void fillBaseValues(const PropertyBag& properties);
	virtual void init(Animator animator, Actor* caster);
	void setAnimations(Animator animator);

	virtual void fillValues(const PropertyBag& properties) { }; // TODO:  = 0;
	virtual void fastUpdate(float dt) = 0;
	virtual void slowUpdate(float dt) = 0;
	virtual void render() = 0;
	virtual void exit();

	virtual AbilityTarget targetType() const = 0;
	virtual AbilityType type() const { return AbilityType::None; } // TODO:  = 0;

	void setCaster(Actor* caster) { mCaster = caster; }
	Actor* caster() const { return mCaster; }

	void setName(const BasicString& name) { mName = name; }
	BasicString name() const { return mName; }

	void setState(AbilityState state) { mState = state; }
	AbilityState state() const { return mState; }

	LocalDispatcher& events() { return mEvents; }
	Cooldown& cooldown() { return mCooldown; }

	virtual bool isRanged() const { return false; }


protected:
	BasicString mName;
	AbilityState mState;

	Animator mAnimator;
	Cooldown mCooldown;
	LocalDispatcher mEvents;

	Actor* mCaster;

	RectF mRect;
	float mMaxDimention;
};