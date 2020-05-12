#pragma once

#include "Objects/Effects/Effect.h"

class Actor;

class Ability
{
public:
	Ability() { }
	virtual ~Ability() { }

	virtual void activate(Actor* target) = 0;
};



class SlowAbility : public Ability
{
public:
	SlowAbility(float slowFactor) : mSlowFactor(slowFactor) { }

	void activate(Actor* target) override;

private:
	float mSlowFactor;
};


class HealAbility : public Ability
{
public:
	HealAbility(float heal) : mHeal(heal) { }

	void activate(Actor* target) override;

private:
	float mHeal;
};