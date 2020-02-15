#pragma once

#include "Animations/Animator.h"


class Swing
{
public:
	Swing() { }
	Swing(double angle, double speed) : mAngle(angle), mSpeed(speed) { mDirection = 1; }

	void flipSide() { mDirection *= -1; }

	double getAngle() { return mAngle; }
	int getDirection() { return mDirection; }

	double getSpeed() { return mSpeed; }

private:
	double mSpeed;
	double mAngle;
	int mDirection;
};