#pragma once

#include "Ability.h"

class SlowAbility : public Ability
{
public:
	SlowAbility(float slowFactor) : mSlowFactor(slowFactor) { }

	void activate(Actor* target) override;
	void slowUpdate(float dt) override { }
	void render() override { }

	const TargetType targetType() const override { return TargetType::Single_Enemy; }

private:
	float mSlowFactor;
};