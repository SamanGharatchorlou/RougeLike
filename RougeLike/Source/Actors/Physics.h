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
	Physics() : mForce(0), mMaxVelocity(0), mDragFactor(0), mMass(1.0f) { };

	void init(float force, float maxVelocity);
	void handleInput(const InputManager* input);
	void fastUpdate(float dt);
	
	// Force
	void resetHasForce() { mHasForce.zero(); }
	void setForce(float force) { mForce; }
	float force() const { return mForce; }

	// Max velocity
	void setVelocity(VectorF velocity) { mVelocity = velocity; }
	void setMaxVelocity(float maxVelocity) { mMaxVelocity = maxVelocity; }
	float maxVelocity() const { return mMaxVelocity; }

	// Accellerate object
	void accellerate(VectorF acceleration);

	// Move object
	void move(VectorF velocity, float dt);
	void move(VectorF movement);
	void move(float dt);

	// Rect
	void setRect(RectF newRect) { mRect = newRect; }
	RectF& rectRef() { return mRect; }
	RectF rect() const { return mRect; }

	// Position
	void setPosition(VectorF position) { mRect.SetCenter(position); }

	// Movement distance
	float maxMovementDistance(float dt) const { return mMaxVelocity * dt; }
	VectorF movementDistance(float dt) const { return mVelocity * dt; }

	float relativeSpeed() const;

	VectorF direction() const;
	
	// Movement info
	bool hasForce() const { return !mHasForce.isZero(); }
	bool isMoving() const { return !mVelocity.isZero(); }

	// soft Reset
	void reset();

	void facePoint(VectorF point);
	void setFlip(SDL_RendererFlip flip) { mFlip = flip; }
	SDL_RendererFlip flip() const { return mFlip; }

private:
	void applyDrag();


private:
	RectF mRect;
	VectorF mVelocity;
	VectorF mAcceleration;

	Vector2D<bool> mHasForce;	

	float mForce;
	float mMaxVelocity;
	float mDragFactor; // 0 is large, 1 is small
	float mMass;

	SDL_RendererFlip mFlip;


	// temp
	bool yes = false;
};