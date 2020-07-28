#include "pch.h"
#include "WeaponStash.h"

#include "Ranged/RangedWeapon.h"
#include "Melee/MeleeWeapon.h"

#include "Graphics/TextureManager.h"



WeaponStash::WeaponStash()
{
	data["empty"] = new WeaponData;

	meleeWeapon = new MeleeWeapon;
}


WeaponStash::~WeaponStash()
{
	delete meleeWeapon;
	meleeWeapon = nullptr;
}


void WeaponStash::init(TextureManager* tm)
{
	std::vector<std::string> filePaths = FileManager::Get()->allFilesInFolder(FileManager::Config_Weapons);

	for (const std::string& filePath : filePaths)
	{
		XMLParser parser;
		parser.parseXML(filePath);

		if (strcmp(parser.rootNode()->name(), "Melee") == 0)
		{
			MeleeWeaponData* weaponData = new MeleeWeaponData;

			// Texture
			std::string weaponName = parser.firstRootNodeValue("Texture");
			weaponData->texture = tm->getTexture(weaponName, FileManager::Image_Weapons);

			fillBasicWeaponData(parser, weaponData);
			fillMeleeWeaponData(parser, weaponData);

			data[weaponName] = weaponData;
		}
	}
}



void WeaponStash::fillBasicWeaponData(XMLParser& parser, WeaponData* data)
{
	ValueMap properties = parser.valueMap(parser.rootNode()->first_node("Properties"));

	// Damage
	float damage = properties["Damage"];
	data->damage = Damage(damage);

	// Cooldown
	data->cooldown = Cooldown(properties["Cooldown"] / 100.0f);

	// Audio
	StringMap audio = parser.stringMap(parser.rootNode()->first_node("Audio"));
	data->audioHit = audio["Hit"];
	data->audioMiss = audio["Miss"];

	// Size and offset
	data->maxDimention = std::stof(parser.firstRootNodeValue("MaxSize"));

	Attributes attributes = parser.attributes(parser.rootNode()->first_node("Offset"));
	int x = attributes.getInt("x");
	int y = attributes.getInt("y");
	data->offset = VectorF(x, y);
}



void WeaponStash::fillMeleeWeaponData(XMLParser& parser, MeleeWeaponData* data)
{
	data->type = WeaponType::Melee;

	ValueMap properties = parser.valueMap(parser.rootNode()->first_node("Properties"));

	// Swing speed & angle
	data->swingSpeed = properties["SwingSpeed"];
	data->swingArc = properties["SwingAngle"];
	data->knockbackDistance = properties["KnockbackDistance"];
	data->knockbackForce = properties["KnockbackForce"];
}


WeaponData* WeaponStash::getData(const std::string& weaponName)
{
#if _DEBUG
	if(data.count(weaponName) == 0)
		DebugPrint(Warning, "There is no weapon with the label %s\n", weaponName.c_str());
#endif
	return data.count(weaponName) > 0 ? data[weaponName] : data["empty"];
}


Weapon* WeaponStash::getWeapon(const std::string& weaponName)
{
	WeaponData* data = getData(weaponName);

	if (data->type == WeaponType::Melee)
	{
		meleeWeapon->equipt(data);
		return meleeWeapon;
	}
	else
	{
		DebugPrint(Warning, "Weapon %s data has no type\n", weaponName.c_str());
		return nullptr;
	}
}