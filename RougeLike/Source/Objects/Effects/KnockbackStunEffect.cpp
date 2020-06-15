#include "pch.h"
#include "KnockbackStunEffect.h"

#include "Objects/Actors/Actor.h"
#include "StunEffect.h"


KnockbackStunEffect::KnockbackStunEffect
	(VectorF source, float distance, float force, Animator animator, float maxSize)
	: DisplacementEffect(source, distance, force), 
	  mStunAnimator(animator), 
	  mMaxStunSize(maxSize)
{

}



void KnockbackStunEffect::slowUpdate(float dt)
{
	VectorF direction = mActor->position() - mSource;
	VectorF velocity = direction.normalise() * mForce;

	if (!canMove(velocity, dt))
	{
		StunEffect* stunEffect = new StunEffect(mStunAnimator, mMaxStunSize);
		mActor->addEffect(stunEffect);
		endEffect();
	}
}