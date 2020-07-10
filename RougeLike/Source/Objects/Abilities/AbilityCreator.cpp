#include "pch.h"
#include "AbilityCreator.h"

#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"

#include "Objects/Abilities/AbilityTypes/Ability.h"
#include "Objects/Abilities/AbilityTypes/SlowAbility.h"
#include "Objects/Abilities/AbilityTypes/HealAbility.h"
#include "Objects/Abilities/AbilityTypes/SpikeAbility.h"
#include "Objects/Abilities/AbilityTypes/BilnkAbility.h"
#include "Objects/Abilities/AbilityTypes/ArmorAbility.h"
#include "Objects/Abilities/AbilityTypes/SmashAbility.h"
#include "Objects/Abilities/AbilityTypes/ChargeAbility.h"


Ability* createNewAbility(const std::string& name, TextureManager* textureManager)
{
	XMLParser parser;
	parser.parseXML(FileManager::Get()->findFile(FileManager::Config_Abilities, name));

	xmlNode propertyNode = parser.rootNode()->first_node("Properties");
	ValueMap values = parser.values(propertyNode);

	Ability* ability = nullptr;

	if (name == "Heal")
	{
		ability = new HealAbility;
	}
	else if (name == "Spikes")
	{
		ability = new SpikeAbility;
	}
	else if (name == "Blink")
	{
		ability = new BlinkAbility;
	}
	else if (name == "Armor")
	{
		ability = new ArmorAbility;
	}
	else if (name == "Smash")
	{
		// Create hammer
		Texture* texture = textureManager->getTexture(values["HammerTexture"], FileManager::Image_Weapons);
		VectorF size = realiseSize(texture->originalDimentions, std::stof(values["HammerMaxSize"]));
		ability = new SmashAbility(texture, size);
	}
	else if (name == "Charge")
	{
		ability = new ChargeAbility;
	}


	// Set Ability values
	if (ability)
	{
		ability->fillAbilityValues(values);
		ability->fillValues(values);
		ability->setName(name);

		// Set ranged ability values
		RangedAbility* rangedAbility = dynamic_cast<RangedAbility*>(ability);
		if (rangedAbility)
		{
			rangedAbility->fillRangedAbilityValues(values);
			setRangeCircle(rangedAbility, textureManager);
		}
	}
	else
		DebugPrint(Warning, "No new ability could be created wth the name '%s'\n", name.c_str());

	return ability;
}


void setRangeCircle(RangedAbility* ability, TextureManager* textureManager)
{
	Texture* rangeCircle = textureManager->getTexture("RangeCircle", FileManager::Image_UI);
	ability->setRangeCircle(rangeCircle);
}