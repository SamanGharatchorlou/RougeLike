#include "pch.h"
#include "KnockbackStunEffect.h"

#include "Objects/Actors/Actor.h"
#include "StunEffect.h"


void KnockbackStunEffect::clearData()
{
	mStunEffect = nullptr;
	DisplacementEffect::clearData();
}


void KnockbackStunEffect::slowUpdate(float dt)
{
	VectorF direction = mReceiver->position() - mSource;
	VectorF velocity = direction.normalise() * mForce;

	if (!canMove(mReceiver, velocity, dt))
	{
		mReceiver->addEffect(mStunEffect);
		endEffect();
	}
}