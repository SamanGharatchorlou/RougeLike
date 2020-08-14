#pragma once

#include "DisplacementEffect.h"
#include "Animations/Animator.h"

class Effect;


// BUG: when hitting a wall jitters
class KnockbackStunEffect : public DisplacementEffect
{
public:
	KnockbackStunEffect() : mStunTime(0.0f) { };

	void fill(const PropertyMap& valueBag) override;


	void slowUpdate(float dt) override;

	EffectType type() const override { return EffectType::KnockbackStun; }

private:
	float mStunTime;
};