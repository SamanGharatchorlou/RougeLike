#pragma once

#include "Ability.h"

class SpikeAbility : public AreaAbility
{
public:
	SpikeAbility() { }

	void fillValues(ValueMap& values) override;

	void activate(VectorF position) override;
	void activate(Actor* actor) override;
	void slowUpdate(float dt) override;

	const TargetType targetType() const override { return TargetType::Area_Attack; }


private:
	Damage mDamage;
	float mForce;
};