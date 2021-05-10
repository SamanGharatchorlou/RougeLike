#include "pch.h"
#include "WeaponStash.h"

#include "Magic/MagicWeapon.h"
#include "Melee/MeleeWeapon.h"



WeaponStash::WeaponStash() { }
WeaponStash::~WeaponStash() { clear(); }


void WeaponStash::load()
{
	FileManager* files = FileManager::Get();
	std::vector<BasicString> filePaths = files->allFilesInFolder(FileManager::Config_Weapons);

	for (const BasicString& filePath : filePaths)
	{
		XMLParser parser(filePath);
		XMLNode root = parser.rootNode();

		WeaponData* weaponData = createNewData(root);
		if (weaponData)
		{
			BasicString weaponName = files->getItemName(filePath);
			mData[weaponName] = weaponData;
		}
	}
}


void WeaponStash::clear()
{
	std::unordered_map<BasicString, WeaponData*>::iterator iter;
	for (iter = mData.begin(); iter != mData.end(); iter++)
	{
		WeaponData* weaponData = iter->second;
		delete weaponData;
	}

	mData.clear();
}


Weapon* WeaponStash::getNewWeapon(WeaponType type) const
{
	Weapon* newWeapon = nullptr;

	if (type == WeaponType::Melee)
		newWeapon = new MeleeWeapon;
	else if (type == WeaponType::Magic)
		newWeapon = new MagicWeapon;
	else
		DebugPrint(Warning, "Invalid Weapon type %d\n", (int)type);

	return newWeapon;
}


WeaponData* WeaponStash::getData(const BasicString& weaponName)
{
#if DEBUG_CHECK
	if(mData.count(weaponName) == 0)
		DebugPrint(Warning, "There is no weapon with the label %s\n", weaponName.c_str());
#endif
	return mData.count(weaponName) > 0 ? mData[weaponName] : nullptr;
}



// -- Private Functions -- //

WeaponData* WeaponStash::createNewData(const XMLNode weaponNode) const
{
	WeaponRawData rawData = getRawData(weaponNode);

	WeaponData* data = nullptr;
	if (rawData.type == "Melee")
		data = new MeleeWeaponData;
	else if (rawData.type == "Magic")
		data = new MagicWeaponData;

	if (data)
		data->fillData(rawData);
	else
		DebugPrint(Warning, "Weapon type '%s' not recognise, cannot create data\n", weaponNode.name().c_str());

	return data;
}


WeaponRawData WeaponStash::getRawData(const XMLNode weaponNode) const
{
	WeaponRawData data;
	data.type = weaponNode.child("Type").value();
	data.properties.fill(weaponNode.child("Properties"));
	data.audio.fill(weaponNode.child("Audio"));
	data.effectData.fill(weaponNode.child("AttackingEffects"));
	return data;
}