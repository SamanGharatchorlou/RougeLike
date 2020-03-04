#pragma once

#include "pch.h"
#include "Characters/Attributes/Damage.h"

class Texture;


struct WeaponData
{
	Texture* texture = nullptr;
	Damage damage;
	VectorF handleOffset;
};


struct MeleeWeaponData : public WeaponData
{
	float swingArc = 0.0;
	float swingSpeed = 0.0;
};


struct RangedWeaponData : public WeaponData
{
	Texture* projectileTexture;
	float travelSpeed;
	VectorF projectileSize;
};