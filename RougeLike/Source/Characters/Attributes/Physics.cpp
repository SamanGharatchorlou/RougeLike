#include "pch.h"
#include "Physics.h"

#include "Input/InputManager.h"

void Physics::init(Data data)
{
	mForce = data.force;
	mMaxVelocity = data.maxVelocity;
	mDragFactor = data.dragFactor;
	mMass = data.mass;
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


void Physics::move(VectorF velocity, float dt)
{
	mRect = mRect.Translate(velocity * dt);
}


// TODO: I think because the force is huge, i.e. instanct accel the normalisation is essentially not doing anything...
// fix me!
// find a work around for instant accelleration?? big units should slowely accellerate though...
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


void Physics::reset()
{
	mRect.SetTopLeft(VectorF());
	mVelocity = VectorF();
	mAcceleration = VectorF();

	resetHasForce();
	resetAllowedMovement();
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


	// Changing direction
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
