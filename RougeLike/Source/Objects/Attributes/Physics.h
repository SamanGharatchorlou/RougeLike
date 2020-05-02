#pragma once

#include "Input/Button.h"

class InputManager;

class Physics
{
public:
	struct Data
	{
		float force = 0.0f;
		float maxVelocity = 0.0f;
		float dragFactor = 0.0f;
		float mass = 0.0f;
	};

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

	void init(Data data);	
	void handleInput(InputManager* input);
	void fastUpdate(float dt);
	
	// Force
	void resetHasForce() { mHasForce.zero(); }
	void setForce(float force) { mForce; }
	float force() const { return mForce; }

	// Max velocity
	void setMaxVelocity(float maxVelocity) { mMaxVelocity = maxVelocity; }
	float maxVelocity() const { return mMaxVelocity; }

	// Accellerate object
	void accellerate(VectorF acceleration);

	// Move object
	void move(VectorF velocity, float dt);

	// Rect
	void setRect(RectF newRect) { mRect = newRect; }
	RectF& rectRef() { return mRect; }
	RectF rect() const { return mRect; }

	// Position
	void setPosition(VectorF position) { mRect.SetCenter(position); }
	VectorF position() const { return mRect.Center(); }

	// Movement distance
	float maxMovementDistance(float dt) const { return mMaxVelocity * dt; }
	VectorF movementDistance(float dt) const { return mVelocity * dt; }

	float relativeSpeed() const;

	VectorF direction() const;
	
	// Movement info
	bool hasForce() const { return !mHasForce.isZero(); }
	bool isMoving() const { return !mVelocity.isZero(); }

	// Movement
	void resetAllowedMovement();
	void restrictMovement(Direction direction, bool restriction);

	// soft Reset
	void reset();


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
	float mDragFactor; // 0 is large, 1 is small
	float mMass;
};