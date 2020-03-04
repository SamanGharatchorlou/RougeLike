#include "pch.h"
#include "WeaponStash.h"

#include "Ranged/RangedWeapon.h"
#include "Melee/MeleeWeapon.h"

#include "Graphics/TextureManager.h"


WeaponStash::WeaponStash()
{
	data["empty"] = new WeaponData;

	meleeWeapon = new MeleeWeapon;
	rangedWeapon = new RangedWeapon;
}


WeaponStash::~WeaponStash()
{
	delete meleeWeapon;
	meleeWeapon = nullptr;

	delete rangedWeapon;
	rangedWeapon = nullptr;
}


void WeaponStash::load(TextureManager* tm)
{
	std::vector<std::string> filePaths = FileManager::Get()->fullPathsInFolder(FileManager::Config_Stats_Weapons);

	for (const std::string& filePath : filePaths)
	{
		XMLParser parser;
		parser.parseXML(filePath);

		if (strcmp(parser.getRootNode()->name(), "Melee") == 0)
		{
			MeleeWeaponData* weaponData = new MeleeWeaponData;

			// Texture
			std::string fileName = FileManager::Get()->fileName(filePath);
			weaponData->texture = tm->getTexture(fileName);

			fillBasicWeaponData(parser, weaponData);
			fillMeleeWeaponData(parser, weaponData);

			ASSERT(Warning, data.count(fileName) == 0, "The file: %s has already been loaded into the weapon stash.cpp\n", filePath);

			data[fileName] = weaponData;
		}
		else if (strcmp(parser.getRootNode()->name(), "Ranged") == 0)
		{
			RangedWeaponData* weaponData = new RangedWeaponData;

			// Texture
			std::string fileName = FileManager::Get()->fileName(filePath);
			weaponData->texture = tm->getTexture(fileName);

			fillBasicWeaponData(parser, weaponData);
			fillRangedWeaponData(parser, weaponData);

			ASSERT(Warning, data.count(fileName) == 0, "The file: %s has already been loaded into the weapon stash.cpp\n", filePath);

			data[fileName] = weaponData;
		}
	}
}



void WeaponStash::fillBasicWeaponData(XMLParser& parser, WeaponData* data)
{
	// Damage
	int damage = std::stoi(parser.getRootNode()->first_node("Damage")->value());
	data->damage = Damage(damage);

	// Pommel offset
	Attributes attributes = parser.getAttributes(parser.getRootNode()->first_node("PommelOffset"));
	float x = attributes.getFloat("x");
	float y = attributes.getFloat("y");
	data->handleOffset.set(x, y);
}



void WeaponStash::fillMeleeWeaponData(XMLParser& parser, MeleeWeaponData* data)
{
	// Swing speed & angle
	data->swingSpeed = std::stod(parser.getRootNode()->first_node("SwingSpeed")->value());
	data->swingArc = std::stod(parser.getRootNode()->first_node("SwingAngle")->value());
}


void WeaponStash::fillRangedWeaponData(XMLParser& parser, RangedWeaponData* data)
{
	// Travel speed
	data->travelSpeed = std::stof(parser.getRootNode()->first_node("TravelSpeed")->value());
}



WeaponData* WeaponStash::getData(std::string label)
{
#if _DEBUG
	if(data.count(label) == 0)
		DebugPrint(Warning, "There is no weapon with the label %s\n", label.c_str());
#endif
	return data.count(label) > 0 ? data[label] : data["empty"];
}