#pragma once

#include "DisplacementEffect.h"
#include "Animations/Animator.h"

class StunEffect;


class KnockbackStunEffect : public DisplacementEffect
{
public:
	KnockbackStunEffect(StunEffect* stunEffect) : mStunEffect(stunEffect) { };

	void clearData();

	void slowUpdate(float dt) override;

	EffectType type() const override { return EffectType::KnockbackStun; }

private:
	StunEffect* mStunEffect;
};