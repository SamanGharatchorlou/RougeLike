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
	WeaponRawData getRawData(const XMLNode rootNode, const TextureManager* textures) const;
	WeaponData* createNewData(const XMLNode rootNode, const WeaponRawData& rawData) const;

private:
	std::unordered_map<BasicString, WeaponData*> mData;

	MeleeWeapon* meleeWeapon;
};