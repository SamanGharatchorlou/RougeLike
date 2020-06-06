#pragma once

#include "Ability.h"

class BlinkAbility: public AreaAbility
{
public:
	BlinkAbility() { }

	void fillValues(ValueMap& values) override;

	void activate(VectorF position) override;
	void activate(Actor* actor) override;
	void slowUpdate(float dt) override;

	const TargetType targetType() const override { return TargetType::Area_Point; }


private:
	VectorF mTargetPosition;
	Timer<float> mTimer;
};