#include "pch.h"
#include "DashEffect.h"

#include "Objects/Actors/Actor.h"
#include "Map/Map.h"


void DashEffect::fastUpdate(float dt)
{
	float distanceToTarget = distanceSquared(mReceiver->position(), mTarget);

	VectorF direction = mReceiver->position();
	VectorF velocity = direction.normalise() * 1000.0f;

	float movementStep = distanceSquared(VectorF(), velocity);

	if (distanceToTarget < movementStep)
		endEffect();
	else if (canMove(mReceiver, velocity, dt))
		mReceiver->physics()->move(velocity, dt);
}


void DashEffect::slowUpdate(float dt)
{

}
