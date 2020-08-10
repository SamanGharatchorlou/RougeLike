#pragma once

#include "WeaponData.h"

class TextureManager;
class Weapon;
class MeleeWeapon;

class WeaponStash
{
public:
	WeaponStash();
	~WeaponStash();

	void load(TextureManager* tm);

	MeleeWeapon* getMeleeWeapon() const { return meleeWeapon; }
	Weapon* getWeapon(const BasicString& weaponType) const;

	WeaponData* getData(const BasicString& weaponName);


private:
	WeaponRawData getRawData(const XMLNode weaponNode, const TextureManager* textures) const;
	WeaponData* createNewData(const XMLNode weaponNode, const WeaponRawData& rawData) const;

private:
	std::unordered_map<BasicString, WeaponData*> mData;

	MeleeWeapon* meleeWeapon;
};