#include "pch.h"
#include "KnockbackStunEffect.h"

#include "Objects/Actors/Actor.h"
#include "StunEffect.h"



KnockbackStunEffect::KnockbackStunEffect(Animator animator) : mMaxStunSize(0.0f) { }


KnockbackStunEffect::KnockbackStunEffect(VectorF source, float distance, float force, Animator animator, float maxSize)
	: DisplacementEffect(source, distance, force), 
	  mStunAnimator(animator), 
	  mMaxStunSize(maxSize)
{

}


void KnockbackStunEffect::clearData()
{
	mStunAnimator.reset();
	mMaxStunSize = 0.0f;

	DisplacementEffect::clearData();
}


void KnockbackStunEffect::init()
{
	mMaxStunSize = mReceiver->rect().Size().x;
}


void KnockbackStunEffect::slowUpdate(float dt)
{
	VectorF direction = mReceiver->position() - mSource;
	VectorF velocity = direction.normalise() * mForce;

	if (!canMove(velocity, dt))
	{
		StunEffect* stunEffect = new StunEffect(mStunAnimator, mMaxStunSize);
		mReceiver->addEffect(stunEffect);
		endEffect();
	}
}