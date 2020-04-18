#include "pch.h"
#include "Movement.h"
#include "Collisions/Collider.h"

void Movement::init(Collider* collider, float speed)
{
	mCollider = collider;
	mSpeed = VectorF(speed, speed);
}

void Movement::fastUpdate(float dt)
{
	mPreviousPosition = mPosition;

	mDirection = clamp(mDirection, -1.0f, +1.0f).normalise();
	mPosition += (mSpeed * mDirection) * dt; // *mCanMove;

	//printf("direction %f, %f\n", mDirection.x, mDirection.y);
}
