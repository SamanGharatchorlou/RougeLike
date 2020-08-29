#pragma once

#include "AbilityStates.h"
#include "Animations/Animator.h"
#include "Objects/Abilities/Cooldown.h"


class Actor;
class EffectPool;
class Collider;
enum class EffectType;


class Ability
{
public:
	Ability();
	virtual ~Ability();

	virtual void init(Actor* caster, const PropertyMap& properties, Animator animator);

	virtual void fastUpdate(float dt) = 0;
	virtual void slowUpdate(float dt) = 0;
	virtual void render();
	virtual void exit() { };
	virtual void baseExit();

	virtual void activate(VectorF position) = 0;
	virtual void activateOn(Actor* actor, EffectPool* effectPool) = 0;

	Actor* caster() const { return mCaster; }
	Cooldown& cooldown() { return mCooldown; }
	Collider* collider() { return mCollider; }
	PropertyMap& properties() { return mProperties; }

	virtual AbilityTarget targetType() const = 0;
	virtual AbilityType type() const = 0;

	void setState(AbilityState state) { mState = state; }
	AbilityState state() const { return mState; }

	bool hasCompleted() const { return mCompleted; }
	bool shouldActivateCollisions() const { return mActivateCollisions; }

	BasicString name() const;



protected:
	void applyEffect(EffectType effectType, Actor* target, EffectPool* effectPool) const;



protected:
	bool mCompleted;
	bool mActivateCollisions;
	AbilityState mState;

	Animator mAnimator;
	Cooldown mCooldown;
	PropertyMap mProperties;

	Actor* mCaster;

	RectF mRect;
	Collider* mCollider;
};