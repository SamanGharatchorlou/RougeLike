#pragma once

#include "KnockbackEffect.h"

class Animator;

class KnockbackStunEffect : public KnockbackEffect
{
public:
	KnockbackStunEffect(const DamageCollider* sourceCollider, Animator* animator, VectorF size);
	void slowUpdate(float dt);

private:
	Animator* mStunAnimator;
	VectorF mStunSize;
};