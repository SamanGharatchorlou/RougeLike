#pragma once

#include "Ability.h"

class HealAbility : public Ability
{
public:
	HealAbility(float heal) : mHeal(heal) { };

	void init(Animator animator) override;
	void activate(Actor* target) override;
	void slowUpdate(float dt) override;
	void render() override;

	const TargetType targetType() const override { return TargetType::Self; }


private:
	Actor* mSelf;
	float mHeal;
	Timer<float> mTimer;
};