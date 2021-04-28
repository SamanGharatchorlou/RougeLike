#pragma once

#include "WeaponData.h"

class Weapon;

class WeaponStash
{
public:
	WeaponStash();
	~WeaponStash();

	void load();
	void clear();

	WeaponData* getData(const BasicString& weaponName);

	Weapon* getNewWeapon(WeaponType type) const;


private:
	WeaponRawData getRawData(const XMLNode weaponNode) const;
	WeaponData* createNewData(const XMLNode weaponNode) const;

private:
	std::unordered_map<BasicString, WeaponData*> mData;
};