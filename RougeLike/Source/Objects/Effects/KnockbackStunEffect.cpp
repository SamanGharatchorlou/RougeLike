#include "pch.h"
#include "KnockbackStunEffect.h"

#include "Objects/Actors/Actor.h"
#include "StunEffect.h"


KnockbackStunEffect::KnockbackStunEffect(const DamageCollider* sourceCollider, Animator* animator, VectorF size) 
	: KnockbackEffect(sourceCollider), mStunAnimator(animator), mStunSize(size)
{

}


void KnockbackStunEffect::slowUpdate(float dt)
{
	if (!canMove)
	{
		StunEffect* stunEffect = new StunEffect(mStunAnimator, mStunSize);
		mActor->addEffect(stunEffect);
	}
}