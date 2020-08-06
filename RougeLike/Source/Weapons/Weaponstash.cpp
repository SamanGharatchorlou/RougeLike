#include "pch.h"
#include "WeaponStash.h"

#include "Graphics/TextureManager.h"
#include "Melee/MeleeWeapon.h"



WeaponStash::WeaponStash()
{
	meleeWeapon = new MeleeWeapon;
}


WeaponStash::~WeaponStash()
{
	std::unordered_map<BasicString, WeaponData*>::iterator iter;
	for (iter = mData.begin(); iter != mData.end(); iter++)
	{
		WeaponData* weaponData = iter->second;
		delete weaponData;
	}
}


void WeaponStash::load(TextureManager* tm)
{
	FileManager* files = FileManager::Get();
	std::vector<BasicString> filePaths = files->allFilesInFolder(FileManager::Config_Weapons);

	for (const BasicString& filePath : filePaths)
	{
		XMLParser parser;
		parser.parseXML(filePath);
		XMLNode root = parser.root();

		BasicString weaponName = files->getItemName(filePath);
		WeaponRawData rawData = getRawData(root, tm);
		WeaponData* weaponData = createNewData(root, rawData);
		if (weaponData)
		{
			BasicString weaponName = files->getItemName(filePath);
			mData[weaponName] = weaponData;
		}
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

WeaponData* WeaponStash::createNewData(const XMLNode rootNode, const WeaponRawData& rawData) const
{
	WeaponData* data = nullptr;

	BasicString name = rootNode.name();
	if (rootNode.name() == "Melee")
	{
		data = new MeleeWeaponData;
	}

	if (data)
		data->fillData(rawData);
	else
		DebugPrint(Warning, "Weapon type '%s' not recognise, cannot create data\n", rootNode.name().c_str());

	return data;
}


WeaponRawData WeaponStash::getRawData(const XMLNode rootNode, const TextureManager* textures) const
{
	WeaponRawData data;
	data.properties = rootNode.first("Properties").stringMap();
	data.audio = rootNode.first("Audio").stringMap();

	EffectBag effectBag;
	effectBag.readEffects(rootNode.first("Effects"));
	data.effects = effectBag;

	// Texture
	BasicString weaponName = data.properties["Texture"];
	data.texture = textures->getTexture(weaponName, FileManager::Image_Weapons);
	
	return data;
}