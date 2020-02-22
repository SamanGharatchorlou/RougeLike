#pragma once
#include "Characters/Attributes/Property.h"
#include "Characters/Attributes/Health.h"

class PlayerPropertyBag
{
public:
	void readAttributes(std::string name);

public:
	HealthProperty pHealth;
	ForceProperty pForce;
	MaxVelocityProperty pMaxVelocity;
	DragFactorProperty pDragFactor;
};


