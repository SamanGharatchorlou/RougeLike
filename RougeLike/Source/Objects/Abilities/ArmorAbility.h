#pragma once

#include "Ability.h"

class ArmorAbility : public Ability
{
public:
	ArmorAbility(float armor) : mArmor(armor) { };

	void init(Animator animator) override;
	void activate(Actor* target) override;
	void slowUpdate(float dt) override;
	void render() override;

	const TargetType targetType() const override { return TargetType::Self; }


private:
	Actor* mSelf;
	float mArmor;

	Timer<float> mTimer;
};