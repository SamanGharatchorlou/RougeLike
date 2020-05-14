#pragma once

#include "Objects/Effects/Effect.h"

class Actor;

class Ability
{
public:
	enum TargetType
	{
		Self,
		Single_Enemy
	};


public:
	Ability() { }
	virtual ~Ability() { }

	virtual void activate(Actor* target) = 0;

	virtual const TargetType targetType() const = 0;
};



class SlowAbility : public Ability
{
public:
	SlowAbility(float slowFactor) : mSlowFactor(slowFactor) { }

	void activate(Actor* target) override;

	const TargetType targetType() const override { return TargetType::Single_Enemy; }

private:
	float mSlowFactor;
};


class HealAbility : public Ability
{
public:
	HealAbility(float heal) : mHeal(heal) { }

	void activate(Actor* target) override;

	const TargetType targetType() const override { return TargetType::Self; }

private:
	float mHeal;
};