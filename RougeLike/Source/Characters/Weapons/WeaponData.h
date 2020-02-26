#pragma once

#include "pch.h"
#include "Characters/Attributes/Damage.h"

class Texture;


struct WeaponData
{
	Texture* texture = nullptr;
	Damage damage;
	double swingArc = 0.0;
	double swingSpeed = 0.0;
	VectorF pommeloffset;
};

