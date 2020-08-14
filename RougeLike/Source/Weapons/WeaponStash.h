#pragma once

#include "WeaponData.h"

class Weapon;
class MeleeWeapon;

class WeaponStash
{
public:
	WeaponStash();
	~WeaponStash();

	void load();

	MeleeWeapon* getMeleeWeapon() const { return meleeWeapon; }
	Weapon* getWeapon(const BasicString& weaponType) const;

	WeaponData* getData(const BasicString& weaponName);


private:
	WeaponRawData getRawData(const XMLNode weaponNode) const;
	WeaponData* createNewData(const XMLNode weaponNode, const WeaponRawData& rawData) const;

private:
	std::unordered_map<BasicString, WeaponData*> mData;

	MeleeWeapon* meleeWeapon;
};