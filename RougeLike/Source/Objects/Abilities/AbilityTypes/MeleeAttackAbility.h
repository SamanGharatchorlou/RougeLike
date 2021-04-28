#pragma once

#include "Objects/Abilities/AbilityClasses/MeleeAbility.h"


class MeleeAttackAbility : public MeleeAbility
{
public:
	~MeleeAttackAbility();
	void activate() override;

	void fastUpdate(float dt) override { } // make not pure virtual?
	void slowUpdate(float dt) override;
	void exit() override;

	AbilityType type() const override { return AbilityType::MeleeAttack; }

private:
	void applyEffects(Actor* actor, EffectPool* effectPool);

};