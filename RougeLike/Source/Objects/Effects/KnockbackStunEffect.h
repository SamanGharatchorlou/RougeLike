#pragma once

#include "KnockbackEffect.h"
#include "Animations/Animator.h"


class KnockbackStunEffect : public KnockbackEffect
{
public:
	//KnockbackStunEffect(const DamageCollider* sourceCollider, Animator* animator, float maxSize);
	KnockbackStunEffect(VectorF source, float force, Animator animator, float maxSize);
	void slowUpdate(float dt);

private:
	Animator mStunAnimator;
	float mMaxStunSize;
};