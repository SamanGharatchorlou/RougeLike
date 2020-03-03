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
	std::vector<std::string> filePaths = FileManager::Get()->fullPathsInFolder(FileManager::Config_Stats_Weapons);

	for (const std::string& filePath : filePaths)
	{
		WeaponData weaponData;

		XMLParser parser;
		parser.parseXML(filePath);
		xmlNode rootNode = parser.getRootNode();

		// Texture
		std::string textureName = rootNode->first_node("Name")->value();
		weaponData.texture = tm->getTexture(textureName);

		// Damage
		int damage = std::stoi(rootNode->first_node("Damage")->value());
		weaponData.damage = Damage(damage);

		// Swing speed & angle
		weaponData.swingSpeed = std::stod(rootNode->first_node("SwingSpeed")->value());
		weaponData.swingArc = std::stod(rootNode->first_node("SwingAngle")->value());

		// Pommel offset
		Attributes attributes = parser.getAttributes(rootNode->first_node("PommelOffset"));
		float x = attributes.getFloat("x");
		float y = attributes.getFloat("y");
		weaponData.pommeloffset.set(x, y);

		std::string fileName = FileManager::Get()->fileName(filePath);
		ASSERT(Warning, data.count(fileName) == 0, "The file: %s has already been loaded into the weapon stash.cpp\n", filePath);

		data[fileName] = weaponData;
	}
}


WeaponData& WeaponStash::getData(std::string label)
{
#if _DEBUG
	if(data.count(label) == 0)
		DebugPrint(Warning, "There is no weapon with the label %s\n", label.c_str());
#endif
	return data.count(label) > 0 ? data[label] : data["empty"];
}