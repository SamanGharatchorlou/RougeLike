#pragma once

#include "pch.h"
#include "Objects/Attributes/Damage.h"

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

	float maxDimention = 0.0f;
	VectorF offset;

	std::string audioMiss;
	std::string audioHit;
};


struct MeleeWeaponData : public WeaponData
{
	float swingArc = 0.0f;
	float swingSpeed = 0.0f;
	float knockbackDistance = 0.0f;
};

