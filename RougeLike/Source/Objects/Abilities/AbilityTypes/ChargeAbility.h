#pragma once

#include "Ability.h"

class ChargeAbility : public AreaAbility
{
public:
	ChargeAbility() { };

	void fillValues(ValueMap& values) override;

	void activate(VectorF position) override;
	void activate(Actor* target, EffectPool* effectPool) override;
	void fastUpdate(float dt) override;
	void slowUpdate(float dt) override;
	void exit() override;

	const TargetType targetType() const override { return TargetType::Area_Point; }


private:
	bool canMove(VectorF velocity, float dt) const;


private:
	EffectPool* mEffectPool;

	// Charge
	bool isCharging;

	VectorF mChargeSource;
	float mChargeForce;
	float mChargeDistance;

	float mDistanceTravelled;

	// Knockback
	Damage mDamage;
	float mKnockbackForce;
	float mKnockbackDistance;

	bool mColliderActiveState;
};