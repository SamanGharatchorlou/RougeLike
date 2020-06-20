#pragma once

#include "Ability.h"
#include "Objects/Attributes/Damage.h"


class SpikeAbility : public AreaAbility
{
public:
	SpikeAbility() : mDamage(0.0f), mKnockbackDistance(0.0f), mKnockbackForce(0.0f) { }

	void fillValues(ValueMap& values) override;

	void activate(VectorF position) override;
	void activate(Actor* actor, EffectPool* effectPool) override;
	void slowUpdate(float dt) override;

	const TargetType targetType() const override { return TargetType::Area_Attack; }


private:
	VectorF mSource;
	Damage mDamage;
	float mKnockbackForce;
	float mKnockbackDistance;
};