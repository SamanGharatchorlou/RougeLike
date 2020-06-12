#include "pch.h"
#include "KnockbackStunEffect.h"

#include "Objects/Actors/Actor.h"
#include "StunEffect.h"


KnockbackStunEffect::KnockbackStunEffect(VectorF source, float force, Animator animator, float maxSize)
	: KnockbackEffect(source, force), mStunAnimator(animator), mMaxStunSize(maxSize)
{

}


void KnockbackStunEffect::slowUpdate(float dt)
{
	KnockbackEffect::slowUpdate(dt);

	VectorF direction = mActor->physics()->position() - mSource;
	VectorF velocity = direction.normalise() * mForce;

	if (!canMove(velocity, dt))
	{
		StunEffect* stunEffect = new StunEffect(mStunAnimator, mMaxStunSize);
		mActor->addEffect(stunEffect);
		endEffect();
	}
}