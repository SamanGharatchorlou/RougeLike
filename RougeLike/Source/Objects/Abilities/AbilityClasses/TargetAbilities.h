#pragma once

#include "Ability.h"
#include "RangedAbility.h"

class EffectPool;


class TargetSelfAbility : public Ability
{
public:
	virtual ~TargetSelfAbility() { }
	virtual void activate(EffectPool* pool) = 0;

	AbilityTarget targetType() const override { return AbilityTarget::Self; }

	virtual void render() override;
};


class TargetActorAbility : public RangedAbility
{
public:
	virtual void activateOn(Actor* target, EffectPool* pool) = 0;

	AbilityTarget targetType() const override { return AbilityTarget::Actor; }

	void render() override { }
};


class TargetPositionAbility : public RangedAbility
{
public:
	virtual void activateAt(VectorF position, EffectPool* pool) = 0;

	virtual AbilityTarget targetType() const override { return AbilityTarget::Position; }

	virtual void render() override;
};


class TargePositionAttackAbility : public TargetPositionAbility
{
public:
	virtual void activateOn(Actor* target, EffectPool* pool) = 0;

	AbilityTarget targetType() const override { return AbilityTarget::AttackArea; }

	virtual void render() override;

	void sendActivateOnRequest();
};
