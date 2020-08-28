#pragma once

#include "Objects/Abilities/AbilityClasses/SelfAbility.h"


class ArmorAbility : public SelfAbility
{
public:
	ArmorAbility() { };

	void fastUpdate(float dt) override { }
	void slowUpdate(float dt) override;

	AbilityType type() const override { return AbilityType::Armor; }

private:
	void applyEffects(Actor* actor, EffectPool* effectPool) override;
};