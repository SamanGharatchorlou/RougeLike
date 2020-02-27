#include "pch.h"
#include "WeaponStash.h"

#include "Game/GameData.h"
#include "Graphics/TextureManager.h"


WeaponStash::WeaponStash()
{
	data["empty"] = WeaponData();
}


void WeaponStash::load(TextureManager* tm)
{
	std::string configFilePath = FileManager::Get()->getFolderPath(FileManager::Config_Weapons);

	for (auto& dir : fs::directory_iterator(configFilePath))
	{
		WeaponData weaponData;

		XMLParser parser;
		parser.parseXML(dir.path().u8string());
		xmlNode rootNode = parser.getRootNode();


		// Texture
		std::string textureName = rootNode->first_node("Name")->value();
		weaponData.texture = tm->getTexture(textureName);

		// Damage
		float damage = std::stof(rootNode->first_node("Damage")->value());
		weaponData.damage = Damage(damage);

		// Swing speed & angle
		weaponData.swingSpeed = std::stod(rootNode->first_node("SwingSpeed")->value());
		weaponData.swingArc = std::stod(rootNode->first_node("SwingAngle")->value());

		// Pommel offset
		Attributes attributes = parser.getAttributes(rootNode->first_node("PommelOffset"));
		float x = attributes.getFloat("x");
		float y = attributes.getFloat("y");
		weaponData.pommeloffset.set(x, y);

		std::string fileName = FileManager::Get()->getFileName(dir.path().u8string());

		ASSERT(Warning, data.count(fileName) == 0, "The file: %s has already been loaded into the weapon stash\n", dir.path().u8string());

		data[fileName] = weaponData;
	}
}


WeaponData& WeaponStash::getData(std::string label)
{
	return data.count(label) > 0 ? data[label] : data["empty"];
}