#include "pch.h"
#include "DisplacementEffect.h"

#include "Actors/Actor.h"


DisplacementEffect::DisplacementEffect() : mDistance(0.0f), mForce(0.0f), mDistanceTravelled(0.0f) { }


void DisplacementEffect::fill(const PropertyMap& propertyMap)
{
	setProperty(PropertyType::KnockbackForce, mForce, propertyMap);
	setProperty(PropertyType::KnockbackDistance, mDistance, propertyMap);		
	
	setProperty(PropertyType::xPosition, mSource.x, propertyMap);
	setProperty(PropertyType::yPosition, mSource.y, propertyMap);
}


void DisplacementEffect::fastUpdate(float dt)
{
	VectorF direction = (mReceiver->position() - mSource).normalise();
	VectorF velocity = direction * mForce;

	if (mDistanceTravelled <= mDistance && canMove(mReceiver, velocity, dt))
	{
		mReceiver->physics()->move(velocity, dt);

		float movementStep = std::sqrt(velocity.magnitudeSquared());
		mDistanceTravelled += movementStep * dt;
	}
	else
	{
		endEffect();
	}
}


void DisplacementEffect::exit()
{
	mSource = VectorF();
	mDistance = 0.0f;
	mForce = 0.0f;
	mDistanceTravelled = 0.0f;
}

