#pragma once

#include "Input/Button.h"

class InputManager;

class Physics
{
public:
	enum Direction
	{
		Up,
		Right,
		Down,
		Left,

		Directions
	};

public:
	Physics() : mForce(0), mMaxVelocity(0), mDragFactor(0) { };

	void init(float force, float maxVelocity, float dragFactor);
	
	void resetHasForce() { mHasForce.zero(); }
	void setForce(float force) { mForce; }
	float force() const { return mForce; }

	void handleInput(InputManager* input);
	void fastUpdate(float dt);

	void accellerate(VectorF acceleration);

	void setRect(RectF newRect) { mRect = newRect; }
	RectF& rectRef() { return mRect; }
	RectF rect() const { return mRect; }

	void setPosition(VectorF position) { mRect.SetCenter(position); }
	VectorF position() const { return mRect.Center(); }

	float maxMovementDistance(float dt) const { return mMaxVelocity * dt; }

	float relativeSpeed() const;

	VectorF direction() const;
	

	bool hasForce() const { return !mHasForce.isZero(); }
	bool isMoving() const { return !mVelocity.isZero(); }

	void resetAllowedMovement();
	void restrictMovement(Direction direction, bool restriction);


private:
	void applyDrag();


private:
	RectF mRect;
	VectorF mVelocity;
	VectorF mAcceleration;

	Vector2D<bool> mHasForce;	
	bool restrictedMovement[Directions];

	float mForce;
	float mMaxVelocity;
	float mDragFactor;
};