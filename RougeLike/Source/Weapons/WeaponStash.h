#pragma once

#include "WeaponData.h"

class TextureManager;
class MeleeWeapon;

class WeaponStash
{
public:
	WeaponStash();
	~WeaponStash();

	void load(TextureManager* tm);

	MeleeWeapon* getMeleeWeapon() const { return meleeWeapon; }

	WeaponData& getData(std::string label);

private:
	std::unordered_map<std::string, WeaponData> data;

	MeleeWeapon* meleeWeapon;
};