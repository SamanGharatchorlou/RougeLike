#pragma once

#include "WeaponData.h"

class TextureManager;
class MeleeWeapon;
class RangedWeapon;

class WeaponStash
{
public:
	WeaponStash();
	~WeaponStash();

	void load(TextureManager* tm);

	MeleeWeapon* getMeleeWeapon() const { return meleeWeapon; }
	RangedWeapon* getRangedWeapon() const { return rangedWeapon; }

	WeaponData* getData(std::string label);


private:
	void fillBasicWeaponData(XMLParser& parser, WeaponData* data);
	void fillMeleeWeaponData(XMLParser& parser, MeleeWeaponData* data);
	void fillRangedWeaponData(XMLParser& parser, RangedWeaponData* data);


private:
	std::unordered_map<std::string, WeaponData*> data;

	MeleeWeapon* meleeWeapon;
	RangedWeapon* rangedWeapon;
};