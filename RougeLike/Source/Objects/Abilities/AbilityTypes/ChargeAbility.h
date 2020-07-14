#pragma once

#include "Ability.h"
#include <unordered_set>

class ChargeAbility : public TargePositionAttackAbility
{
public:
	ChargeAbility() { };


	void fillValues(ValueMap& values) override;

	void activateAt(VectorF position, EffectPool* effectPool) override;
	void activateOn(Actor* actor, EffectPool* effectPool) override;

	void fastUpdate(float dt) override;
	void slowUpdate(float dt) override;
	void exit() override;

	void render() override;

private:/*
	bool canMove(VectorF velocity, float dt) const;*/

	void applyEffects(Actor* actor, EffectPool* effectPool);


private:
	VectorF mChargeSource;
	float mChargeForce;
	float mChargeDistance;

	float mDistanceTravelled;

	// Knockback
	Damage mDamage;
	float mKnockbackForce;
	float mKnockbackDistance;

	std::unordered_set<Actor*> mHitList;

	// TEMP
	VectorF start;
	VectorF end;
};