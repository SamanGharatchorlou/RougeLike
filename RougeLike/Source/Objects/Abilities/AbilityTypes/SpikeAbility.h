#pragma once


#include "Objects/Abilities/AbilityClasses/TargetAbilities.h"


class SpikeAbility : public TargePositionAttackAbility
{
public:
	SpikeAbility() { }

	void activateAt(VectorF position, EffectPool* effectPool) override;
	void activateOn(Actor* actor, EffectPool* effectPool) override;

	void fastUpdate(float dt) override { }
	void slowUpdate(float dt) override;

	AbilityType type() const override { return AbilityType::Spikes; }


private:
	void applyEffects(Actor* actor, EffectPool* effectPool);


private:
	VectorF mSource;
};