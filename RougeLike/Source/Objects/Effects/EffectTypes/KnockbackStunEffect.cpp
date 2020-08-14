#include "pch.h"
#include "KnockbackStunEffect.h"

#include "Actors/Actor.h"
#include "Actors/Enemies/Enemy.h"
#include "StunEffect.h"



void KnockbackStunEffect::fill(const PropertyMap& valueBag)
{
	DisplacementEffect::fill(valueBag);
	setProperty(PropertyType::StunTime, mStunTime, valueBag);
}


void KnockbackStunEffect::slowUpdate(float dt)
{
	VectorF direction = mReceiver->position() - mSource;
	VectorF velocity = direction.normalise() * mForce;

	if (!canMove(mReceiver, velocity, dt))
	{
		Enemy* enemy = static_cast<Enemy*>(mReceiver);
		if (enemy)
			enemy->stun(mStunTime);

		endEffect();
	}
}