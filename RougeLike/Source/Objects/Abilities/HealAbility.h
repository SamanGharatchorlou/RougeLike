#pragma once

#include "Ability.h"

class Player;

class HealAbility : public Ability
{
public:
	HealAbility() { };

	void fillValues(ValueMap& values) override;

	void init(Animator animator, Player* player) override;
	void activate(Actor* target) override;
	void slowUpdate(float dt) override;

	const TargetType targetType() const override { return TargetType::Self; }


private:
	float mHeal;
};