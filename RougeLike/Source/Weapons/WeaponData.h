#pragma once

#include "pch.h"
#include "Characters/Attributes/Damage.h"

class Texture;

enum class WeaponType
{
	None,
	Melee,
	Ranged
};

struct WeaponData
{
	WeaponType type = WeaponType::None;
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
	Texture* projectileTexture = nullptr;
	float travelSpeed = 0.0f;
	VectorF projectileSize;
	int quiverSize = 0;
};