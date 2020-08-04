#pragma once


#include "Objects/Abilities/AbilityClasses/TargetAbilities.h"


class SpikeAbility : public TargePositionAttackAbility
{
public:
	SpikeAbility() : mDamage(0.0f), mKnockbackDistance(0.0f), mKnockbackForce(0.0f) { }

	//void fillValues(ValueMap& values) override;

	void activateAt(VectorF position, EffectPool* effectPool) override;
	void activateOn(Actor* actor, EffectPool* effectPool) override;

	void fastUpdate(float dt) override { }
	void slowUpdate(float dt) override;


private:
	void applyEffects(Actor* actor, EffectPool* effectPool);


private:
	VectorF mSource;
	Damage mDamage;
	float mKnockbackForce;
	float mKnockbackDistance;
};