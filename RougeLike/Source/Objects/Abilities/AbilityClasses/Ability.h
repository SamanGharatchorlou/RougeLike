#pragma once

#include "AbilityStates.h"
#include "Events/LocalDispatcher.h"
#include "Animations/Animator.h"
#include "Objects/Abilities/Cooldown.h"
#include "Collisions/Colliders/Collider.h"


class Actor;
class PropertyMap;
class EffectPool;
class Collider;
enum class EffectType;


class Ability
{
public:
	Ability() : mState(AbilityState::None), mCaster(nullptr) { }
	virtual ~Ability();

	virtual void init(Actor* caster, const PropertyMap& properties, Animator animator);

	virtual void fastUpdate(float dt) = 0;
	virtual void slowUpdate(float dt) = 0;
	virtual void render() = 0;
	virtual void exit();

	void renderAnimator();

	Actor* caster() const { return mCaster; }
	LocalDispatcher& events() { return mEvents; }
	Cooldown& cooldown() { return mCooldown; }
	Collider* collider() { return mCollider; }

	virtual AbilityTarget targetType() const = 0;
	virtual AbilityType type() const = 0;

	void setState(AbilityState state) { mState = state; }
	AbilityState state() const { return mState; }

	void applyEffect(EffectType effectType, Actor* target, EffectPool* effectPool) const;

	bool hasCompleted() const { return mCompleted; }

	BasicString name() const;


protected:
	AbilityState mState;

	Animator mAnimator;
	Cooldown mCooldown;
	LocalDispatcher mEvents;

	Actor* mCaster;

	RectF mRect;

	PropertyMap mProperties;

	bool mCompleted;

	Collider* mCollider;
};