#pragma once

#include "AbilityStates.h"
#include "Events/LocalDispatcher.h"
#include "Animations/Animator.h"
#include "Objects/Abilities/Cooldown.h"


class Actor;
class PropertyMap;


class Ability
{
public:
	Ability() : mState(AbilityState::None), mCaster(nullptr) { }
	virtual ~Ability() { }

	void init(const BasicString& name, Actor* caster, const PropertyMap& properties, Animator animator);

	virtual void fastUpdate(float dt) = 0;
	virtual void slowUpdate(float dt) = 0;
	virtual void render() = 0;
	virtual void exit();

	virtual AbilityTarget targetType() const = 0;
	virtual AbilityType type() const { return AbilityType::None; } // TODO:  = 0;

	Actor* caster() const { return mCaster; }

	BasicString name() const { return mName; }

	void setState(AbilityState state) { mState = state; }
	AbilityState state() const { return mState; }

	LocalDispatcher& events() { return mEvents; }
	Cooldown& cooldown() { return mCooldown; }

	virtual bool isRanged() const { return false; }

	bool hasCompleted() const { return mCompleted; }

protected:
	BasicString mName;
	AbilityState mState;

	Animator mAnimator;
	Cooldown mCooldown;
	LocalDispatcher mEvents;

	Actor* mCaster;

	RectF mRect;

	PropertyMap mProperties;

	bool mCompleted;
};