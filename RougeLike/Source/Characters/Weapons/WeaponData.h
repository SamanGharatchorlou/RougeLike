#pragma once

#include "pch.h"
#include "Characters/Attributes/Damage.h"

class Texture;

//class Swing
//{
//public:
//	Swing() : mSpeed(0.0), mAngle(0.0), mDirection(0) { }
//	Swing(double angle, double speed) : mAngle(angle), mSpeed(speed), mDirection(1) { }
//
//	void flipSide() { mDirection *= -1; }
//
//	double angle() const { return mAngle; }
//	double speed() const { return mSpeed; }
//
//private:
//	double mSpeed;
//	double mAngle;
//};



struct WeaponData
{
	Texture* texture = nullptr;
	Damage damage;
	double swingArc = 0.0;
	double swingSpeed = 0.0;
	VectorF pommeloffset;
};

