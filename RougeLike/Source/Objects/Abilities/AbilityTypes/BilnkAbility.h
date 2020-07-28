#pragma once

#include "Ability.h"


class BlinkAbility: public TargetPositionAbility
{
public:
	BlinkAbility() { }

	//void fillValues(ValueMap& values) override { };

	void activateAt(VectorF position, EffectPool* pool) override;

	void fastUpdate(float) override { }
	void slowUpdate(float dt) override;


private:
	void applyEffects(EffectPool* pool);


private:
	VectorF mTargetPosition;
};