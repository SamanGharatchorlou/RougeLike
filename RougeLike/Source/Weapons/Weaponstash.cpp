#include "pch.h"
#include "WeaponStash.h"

#include "Graphics/TextureManager.h"
#include "Melee/MeleeWeapon.h"



WeaponStash::WeaponStash() : meleeWeapon(nullptr) { }


WeaponStash::~WeaponStash()
{
	clear();
}


void WeaponStash::load()
{
	FileManager* files = FileManager::Get();
	std::vector<BasicString> filePaths = files->allFilesInFolder(FileManager::Config_Weapons);

	for (const BasicString& filePath : filePaths)
	{
		XMLParser parser;
		parser.parseXML(filePath);
		XMLNode root = parser.rootNode();

		BasicString weaponName = files->getItemName(filePath);
		WeaponRawData rawData = getRawData(root);
		WeaponData* weaponData = createNewData(root, rawData);
		if (weaponData)
		{
			BasicString weaponName = files->getItemName(filePath);
			mData[weaponName] = weaponData;
		}
	}


	meleeWeapon = new MeleeWeapon;
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

	if (meleeWeapon)
	{
		delete meleeWeapon;
		meleeWeapon = nullptr;
	}
}


WeaponData* WeaponStash::getData(const BasicString& weaponName)
{
#if _DEBUG
	if(mData.count(weaponName) == 0)
		DebugPrint(Warning, "There is no weapon with the label %s\n", weaponName.c_str());
#endif
	return mData.count(weaponName) > 0 ? mData[weaponName] : nullptr;
}


Weapon* WeaponStash::getWeapon(const BasicString& weaponType) const
{
	if (weaponType == "Melee")
		return meleeWeapon;
	else
	{
		DebugPrint(Warning, "Weapon type '%s' not recognised\n", weaponType);
		return nullptr;	
	}
}



// -- Private Functions -- //

WeaponData* WeaponStash::createNewData(const XMLNode weaponNode, const WeaponRawData& rawData) const
{
	WeaponData* data = nullptr;
	if (weaponNode.name() == "Melee")
		data = new MeleeWeaponData;

	if (data)
		data->fillData(rawData);
	else
		DebugPrint(Warning, "Weapon type '%s' not recognise, cannot create data\n", weaponNode.name().c_str());

	return data;
}


WeaponRawData WeaponStash::getRawData(const XMLNode weaponNode) const
{
	WeaponRawData data;
	data.properties = StringMap(weaponNode.child("Properties"));
	data.audio = StringMap(weaponNode.child("Audio"));

	EffectMap EffectMap;
	EffectMap.fill(weaponNode.child("Effects"));
	data.effects = EffectMap;

	// Texture
	const TextureManager* textures = TextureManager::Get();
	BasicString weaponName = data.properties["Texture"];
	data.texture = textures->getTexture(weaponName, FileManager::Image_Weapons);
	
	return data;
}