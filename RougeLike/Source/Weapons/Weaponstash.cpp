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

		if (strcmp(parser.rootNode()->name(), "Melee") == 0)
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
		else if (strcmp(parser.rootNode()->name(), "Ranged") == 0)
		{
			RangedWeaponData* weaponData = new RangedWeaponData;

			// Texture
			std::string fileName = FileManager::Get()->fileName(filePath);
			weaponData->texture = tm->getTexture(fileName);

			fillBasicWeaponData(parser, weaponData);
			fillRangedWeaponData(parser, weaponData, tm);

			ASSERT(Warning, data.count(fileName) == 0, "The file: %s has already been loaded into the weapon stash\n", filePath.c_str());

			data[fileName] = weaponData;
		}
	}
}



void WeaponStash::fillBasicWeaponData(XMLParser& parser, WeaponData* data)
{
	// Damage
	int damage = std::stoi(parser.firstRootNodeValue("Damage"));
	data->damage = Damage(damage);

	// TODO: change this to regular format or keep attribute format?
	// Pommel offset
	Attributes attributes = parser.attributes(parser.rootNode()->first_node("PommelOffset"));
	float x = attributes.getFloat("x");
	float y = attributes.getFloat("y");
	data->handleOffset.set(x, y);
}



void WeaponStash::fillMeleeWeaponData(XMLParser& parser, MeleeWeaponData* data)
{
	data->type = WeaponType::Melee;

	// Swing speed & angle
	data->swingSpeed = std::stof(parser.firstRootNodeValue("SwingSpeed"));
	data->swingArc = std::stof(parser.firstRootNodeValue("SwingAngle"));
	data->knockbackDistance = std::stof(parser.firstRootNodeValue("KnockbackDistance"));
}


void WeaponStash::fillRangedWeaponData(XMLParser& parser, RangedWeaponData* data, TextureManager* tm)
{
	data->type = WeaponType::Ranged;

	// Travel speed
	data->travelSpeed = std::stof(parser.firstRootNodeValue("TravelSpeed"));

	// Projectile texture
	std::string projectileTexture = parser.firstRootNodeValue("ProjectileTexture");
	data->projectileTexture = tm->getTexture(projectileTexture);

	// Projectile size
	float width = std::stof(parser.firstRootNodeValue("ProjectileWidth"));
	float height = std::stof(parser.firstRootNodeValue("ProjectileHeight"));
	data->projectileSize = VectorF(width, height);

	// Quiver size
	data->quiverSize = std::stoi(parser.firstRootNodeValue("QuiverSize"));
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
	else if (data->type == WeaponType::Ranged)
	{
		rangedWeapon->equipt(data);
		return rangedWeapon;
	}
	else
	{
		DebugPrint(Warning, "Weapon %s data has no type\n", weaponName.c_str());
		return nullptr;
	}
}