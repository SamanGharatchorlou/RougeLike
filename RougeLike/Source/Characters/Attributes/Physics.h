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

	void handleInput(InputManager* input);
	void update(float dt);


	void setRect(RectF newRect) { rect = newRect; }
	RectF& getRect() { return rect; }

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
	RectF rect;
	VectorF mVelocity;
	VectorF mAcceleration;

	Vector2D<bool> mHasForce;	
	bool restrictedMovement[Directions];

	float mForce;
	float mMaxVelocity;
	float mDragFactor;
};