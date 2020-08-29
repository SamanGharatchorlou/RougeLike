#pragma once

#include "Objects/Abilities/AbilityClasses/MeleeAbility.h"


class BasicAttackAbility : public MeleeAbility
{
public:
	void activate(VectorF position) override;

	void fastUpdate(float dt) override { } // make not pure virtual?
	void slowUpdate(float dt) override;

	AbilityType type() const override { return AbilityType::Attack; }

private:
	void applyEffects(Actor* actor, EffectPool* effectPool);

};