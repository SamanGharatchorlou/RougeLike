#include "pch.h"
#include "Physics.h"

#include "Input/InputManager.h"


void Physics::init(float force, float maxVelocity)
{
	mForce = force;
	mMaxVelocity = maxVelocity;
	mDragFactor = 0.85f;
}


void Physics::handleInput(const InputManager* input)
{
	mAcceleration.zero();
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

	if (isnan(mVelocity.x))
		printf("waht?\n");

	applyDrag();
}


void Physics::move(VectorF velocity, float dt)
{
	mRect = mRect.Translate(velocity * dt);
}

void Physics::move(float dt)
{
	mRect = mRect.Translate(mVelocity * dt);
}

void Physics::move(VectorF movement)
{
	mRect = mRect.Translate(movement);
}


// TODO: I think because the force is huge, i.e. instanct accel the normalisation is essentially not doing anything...
// fix me!
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
	mFlip = SDL_FLIP_NONE;

	resetHasForce();
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

	if (mVelocity.x > 10000)
		printf("waht?\n");
}


void Physics::facePoint(VectorF point)
{
	if (point.x > mRect.Center().x)		// -->
		mFlip = SDL_FLIP_NONE;
	else							// <--
		mFlip = SDL_FLIP_HORIZONTAL;
}