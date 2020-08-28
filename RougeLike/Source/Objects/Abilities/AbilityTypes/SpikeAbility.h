#pragma once


#include "Objects/Abilities/AbilityClasses/RangedAbility.h"


class SpikeAbility : public TargetAreaRangedAbility
{
public:
	SpikeAbility() { }

	void activate(VectorF position) override;

	void fastUpdate(float dt) override { }
	void slowUpdate(float dt) override;

	AbilityType type() const override { return AbilityType::Spikes; }


private:
	void applyEffects(Actor* actor, EffectPool* effectPool) override;

};