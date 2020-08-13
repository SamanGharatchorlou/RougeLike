#pragma once


#include "Objects/Abilities/AbilityClasses/TargetAbilities.h"


class BlinkAbility: public TargetPositionAbility
{
public:
	BlinkAbility() { }

	void activateAt(VectorF position, EffectPool* pool) override;

	void fastUpdate(float) override { }
	void slowUpdate(float dt) override;

	AbilityType type() const override { return AbilityType::Blink; }

private:
	void applyEffects(EffectPool* pool);


private:
	VectorF mTargetPosition;
};