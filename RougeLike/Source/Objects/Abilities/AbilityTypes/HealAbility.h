#pragma once

#include "Ability.h"
#include "Objects/Attributes/Health.h"


class HealAbility : public TargetSelfAbility
{
public:
	HealAbility() { };

	void fillValues(ValueMap& values) override;
	
	void activate(EffectPool* pool) override;

	void fastUpdate(float dt) override { }
	void slowUpdate(float dt) override;


protected:
	void applyEffects(EffectPool* pool);
	void updateUI();


private:
	Health mHeal;
};