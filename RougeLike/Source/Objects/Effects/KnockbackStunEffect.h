#pragma once

#include "DisplacementEffect.h"
#include "Animations/Animator.h"


class KnockbackStunEffect : public DisplacementEffect
{
public:
	KnockbackStunEffect(Animator animator);
	KnockbackStunEffect(VectorF source, float distance, float force, Animator animator, float maxSize);

	void clearData();

	void init() override;
	void slowUpdate(float dt) override;

private:
	Animator mStunAnimator;
	float mMaxStunSize;
};