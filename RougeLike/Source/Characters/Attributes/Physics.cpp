#include "pch.h"
#include "Physics.h"

#include "Input/InputManager.h"

void Physics::init(float force, float maxVelocity, float dragFactor)
{
	mForce = force;
	mMaxVelocity = maxVelocity;
	mDragFactor = dragFactor;
}


void Physics::handleInput(InputManager* input)
{
	mHasForce.zero();

	// Movement
	if (input->isHeld(Button::Up))
	{
		mAcceleration.y -= 1.0f;
		mHasForce.y = true;
	}

	if (input->isHeld(Button::Down))
	{
		mAcceleration.y += 1.0f;
		mHasForce.y = true;
	}

	if (input->isHeld(Button::Left))
	{
		mAcceleration.x -= 1.0f;
		mHasForce.x = true;
	}

	if (input->isHeld(Button::Right))
	{
		mAcceleration.x += 1.0f;
		mHasForce.x = true;
	}

	mAcceleration = mAcceleration.normalise() * mForce;
}



void Physics::fastUpdate(float dt)
{
	mVelocity += mAcceleration * dt;
	mVelocity = clamp(mVelocity, -mMaxVelocity, mMaxVelocity);

	applyDrag();

	if (restrictedMovement[Direction::Up])
	{
		mVelocity.y = clamp(mVelocity.y, 0.0f, std::abs(mVelocity.y));
	}

	if (restrictedMovement[Direction::Down])
	{
		mVelocity.y = clamp(mVelocity.y, -std::abs(mVelocity.y), 0.0f);
	}

	if (restrictedMovement[Direction::Right])
	{
		mVelocity.x = clamp(mVelocity.x, -std::abs(mVelocity.x), 0.0f);
	}

	if (restrictedMovement[Direction::Left])
	{
		mVelocity.x = clamp(mVelocity.x, 0.0f, std::abs(mVelocity.x));
	}

	mRect = mRect.Translate(mVelocity * dt);

	mAcceleration.zero();
}


void Physics::accellerate(VectorF acceleration)
{
	if (acceleration.x != 0.0f)
	{
		mHasForce.x = true;
	}

	if (acceleration.y != 0.0f)
	{
		mHasForce.y = true;
	}

	mAcceleration = acceleration;
	mAcceleration = clamp(mAcceleration, -1.0f, +1.0f);
	mAcceleration = mAcceleration.normalise() * mForce;
}


void Physics::resetAllowedMovement()
{
	for (int i = 0; i < Direction::Directions; i++)
	{
		restrictedMovement[i] = false;
	}
}


void Physics::restrictMovement(Physics::Direction direction, bool restriction)
{
	restrictedMovement[direction] = restriction;
}


VectorF Physics::direction() const
{
	return clamp(mVelocity, -1.0f, +1.0f);
}


float Physics::relativeSpeed() const
{ 
	return mVelocity.magnitudeSquared() / (mMaxVelocity * mMaxVelocity); 
}


// --- Private Functions --- //
void Physics::applyDrag()
{
	// No movement
	if (!mHasForce.x)
	{
		mVelocity.x = mVelocity.x * mDragFactor;

		if (mVelocity.x < mMaxVelocity * 0.01)
			mVelocity.x = 0;
	}

	if (!mHasForce.y)
	{
		mVelocity.y = mVelocity.y * mDragFactor;

		if (mVelocity.y < mMaxVelocity * 0.01)
			mVelocity.y = 0;
	}


	// Chaning direction
	if (mVelocity.x > 0.0f && mAcceleration.x < 0.0f ||
		mVelocity.x < 0.0f && mAcceleration.x > 0.0f)
	{
		mVelocity.x = mVelocity.x * mDragFactor;
	}

	if (mVelocity.y > 0.0f && mAcceleration.y < 0.0f ||
		mVelocity.y < 0.0f && mAcceleration.y > 0.0f)
	{
		mVelocity.y = mVelocity.y * mDragFactor;
	}
}
