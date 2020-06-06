#pragma once

#include "WeaponData.h"

class TextureManager;
class Weapon;
class MeleeWeapon;
//class RangedWeapon;

class WeaponStash
{
public:
	WeaponStash();
	~WeaponStash();

	void load(TextureManager* tm);

	MeleeWeapon* getMeleeWeapon() const { return meleeWeapon; }
	//RangedWeapon* getRangedWeapon() const { return rangedWeapon; }

	WeaponData* getData(const std::string& weaponName);

	Weapon* getWeapon(const std::string& weaponName);

private:
	void fillBasicWeaponData(XMLParser& parser, WeaponData* data);
	void fillMeleeWeaponData(XMLParser& parser, MeleeWeaponData* data);
	//void fillRangedWeaponData(XMLParser& parser, RangedWeaponData* data, TextureManager* tm);


private:
	std::unordered_map<std::string, WeaponData*> data;

	MeleeWeapon* meleeWeapon;
	//RangedWeapon* rangedWeapon;
};