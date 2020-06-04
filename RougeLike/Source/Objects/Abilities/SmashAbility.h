#pragma once

#include "Ability.h"

class SmashAbility : public Ability
{
public:
	SmashAbility() { };

	void fillValues(ValueMap& values) override;

	void activate(Actor* target) override;
	void slowUpdate(float dt) override;

	const TargetType targetType() const override { return TargetType::Self; }


private:
	Actor* mSelf;
};