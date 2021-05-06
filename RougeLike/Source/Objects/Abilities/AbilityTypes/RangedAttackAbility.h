#pragma once

#include "Objects/Abilities/AbilityClasses/RangedAbility.h"


class RangedAttackAbility : public RangedAbility
{
public:


	void activate() override;

	void fastUpdate(float dt) override { } // make not pure virtual?
	void slowUpdate(float dt) override;
	//void exit() override;

	AbilityType type() const override { return AbilityType::MeleeAttack; }


	bool isValidTarget(VectorF target, const Map* map) const { return true; }

private:
	void applyEffects(Actor* actor, EffectPool* effectPool);

};
