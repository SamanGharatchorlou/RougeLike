#include "pch.h"
#include "DisplacementEffect.h"

#include "Actors/Actor.h"


DisplacementEffect::DisplacementEffect() : mDistance(0.0f), mForce(0.0f), mDistanceTravelled(0.0f) { }


void DisplacementEffect::fill(const PropertyBag& propertyBag)
{
	setProperty(PropertyType::KnockbackForce, mForce, propertyBag);
	setProperty(PropertyType::KnockbackDistance, mDistance, propertyBag);
}


void DisplacementEffect::set(VectorF source, float force, float distance)
{
	mSource = source;
	mForce = force;
	mDistance = distance;
}


void DisplacementEffect::fastUpdate(float dt)
{
	VectorF direction = (mReceiver->position() - mSource).normalise();
	VectorF velocity = direction * mForce;

	if (mDistanceTravelled >= mDistance)
	{
		endEffect();
	}
	else if (canMove(mReceiver, velocity, dt))
	{
		mReceiver->physics()->move(velocity, dt);

		float movementStep = std::sqrt(velocity.magnitudeSquared());
		mDistanceTravelled += movementStep * dt;
	}
}


void DisplacementEffect::exit()
{
	mSource = VectorF();
	mDistance = 0.0f;
	mForce = 0.0f;
	mDistanceTravelled = 0.0f;
}

