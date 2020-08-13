#pragma once

#include "Objects/Abilities/AbilityClasses/TargetAbilities.h"
#include "Objects/Properties/Attributes/Health.h"


class HealAbility : public TargetSelfAbility
{
public:
	HealAbility() { };

	//void set(const PropertyMap& values) override;
	
	void activate(EffectPool* pool) override;

	void fastUpdate(float dt) override { }
	void slowUpdate(float dt) override;

	AbilityType type() const override { return AbilityType::Heal; }

protected:
	void applyEffects(EffectPool* pool);
	void updateUI();


private:
	//Health mHeal;
};