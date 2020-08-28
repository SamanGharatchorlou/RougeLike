#pragma once

#include "Objects/Abilities/AbilityClasses/SelfAbility.h"


class HealAbility : public SelfAbility
{
public:
	HealAbility() { };

	void fastUpdate(float dt) override { } // make not pure virtual?
	void slowUpdate(float dt) override;

	AbilityType type() const override { return AbilityType::Heal; }

private:
	void applyEffects(Actor* actor, EffectPool* effectPool) override;
};