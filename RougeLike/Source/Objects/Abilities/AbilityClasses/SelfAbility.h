#pragma once

#include "Ability.h"

class EffectPool;

class SelfAbility : public Ability
{
public:
	virtual ~SelfAbility() { }

	virtual void activate(VectorF position) override { };
	virtual void activateOn(Actor* actor, EffectPool* effectPool);

	AbilityTarget targetType() const override { return AbilityTarget::Self; }


private:
	virtual void applyEffects(Actor* actor, EffectPool* effectPool) = 0;

};




