#include "pch.h"
#include "BlinkEffect.h"

#include "Actors/Actor.h"


void BlinkEffect::fill(const PropertyMap& propertyMap)
{
	if (propertyMap.contains(PropertyType::xPosition) &&
		propertyMap.contains(PropertyType::yPosition))
	{
		setProperty(PropertyType::xPosition, mTarget.x, propertyMap);
		setProperty(PropertyType::yPosition, mTarget.y, propertyMap);
	}
}


void BlinkEffect::slowUpdate(float dt)
{
	float speed = 2500.0f;
	VectorF direction = (mTarget - mReceiver->position()).normalise();

	float movementStep = distanceSquared(VectorF(), direction * speed * dt);
	float distanceToTarget = distanceSquared(mReceiver->position(), mTarget);

	if (distanceToTarget < movementStep)
	{
		VectorF finalStep = mTarget - mReceiver->position();
		mReceiver->physics()->move(finalStep / dt, dt);
		endEffect();
	}
	else
		mReceiver->physics()->move(direction * speed, dt);
}


void BlinkEffect::exit()
{
	mTarget = VectorF();
}
