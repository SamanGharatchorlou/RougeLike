#pragma once

#include "Ability.h"
#include "Objects/Attributes/Health.h"


class HealAbility : public Ability
{
public:
	HealAbility() { };

	void fillValues(ValueMap& values) override;

	void activate(Actor* target, EffectPool* effectPool) override;
	void slowUpdate(float dt) override;

	const TargetType targetType() const override { return TargetType::Self; }


private:
	Health mHeal;
};