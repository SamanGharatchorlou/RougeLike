#pragma once

#include "Ability.h"

class ArmorAbility : public Ability
{
public:
	ArmorAbility() { };

	void fillValues(ValueMap& values) override;

	void activate(Actor* target) override;
	void slowUpdate(float dt) override;

	const TargetType targetType() const override { return TargetType::Self; }


private:
	float mArmor;

	Actor* mSelf;
	Timer<float> mTimer;
};