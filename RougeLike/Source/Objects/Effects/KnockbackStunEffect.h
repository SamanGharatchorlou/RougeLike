#pragma once

#include "DisplacementEffect.h"
#include "Animations/Animator.h"


class KnockbackStunEffect : public DisplacementEffect
{
public:
	KnockbackStunEffect(VectorF source, float distance, float force, Animator animator, float maxSize);
	void slowUpdate(float dt);

private:
	Animator mStunAnimator;
	float mMaxStunSize;
};