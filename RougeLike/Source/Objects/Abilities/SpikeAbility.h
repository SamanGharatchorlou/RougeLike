#pragma once

#include "Ability.h"

class SpikeAbility : public AreaAbility
{
public:
	SpikeAbility(Damage damage, float force) : mDamage(damage), mForce(force) { }

	void init(Animator animator) override;
	void activate(VectorF position) override;
	void activate(Actor* actor) override;
	void slowUpdate(float dt) override;
	void render() override;

	const TargetType targetType() const override { return TargetType::Area_Attack; }


private:
	Damage mDamage;
	float mForce;
};