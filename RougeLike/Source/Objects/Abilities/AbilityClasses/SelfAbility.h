#pragma once

#include "Ability.h"

class EffectPool;

class SelfAbility : public Ability
{
public:
	virtual ~SelfAbility() { }

	void init();
	virtual bool initiate(const InputManager* input) const { return true; }

	virtual void activate() override { };
	virtual bool activateOn(Actor* actor, EffectPool* effectPool);

	AbilityTarget targetType() const override { return AbilityTarget::Self; }


private:
	virtual void playAudio() = 0;
	virtual void applyEffects(Actor* actor, EffectPool* effectPool) = 0;
};




