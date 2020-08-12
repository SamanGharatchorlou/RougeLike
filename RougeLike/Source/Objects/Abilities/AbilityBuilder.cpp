#include "pch.h"
#include "AbilityBuilder.h"

#include "Graphics/TextureManager.h"
#include "Animations/AnimationReader.h"

// Abilities
#include "Objects/Abilities/AbilityTypes/SlowAbility.h"
#include "Objects/Abilities/AbilityTypes/HealAbility.h"
#include "Objects/Abilities/AbilityTypes/SpikeAbility.h"
#include "Objects/Abilities/AbilityTypes/BilnkAbility.h"
#include "Objects/Abilities/AbilityTypes/ArmorAbility.h"
#include "Objects/Abilities/AbilityTypes/SmashAbility.h"
#include "Objects/Abilities/AbilityTypes/ChargeAbility.h"


Ability* AbilityBuilder::build(const BasicString& id)
{
	Ability* ability = createNewAbility(id);

	XMLParser parser;
	parser.parseXML(FileManager::Get()->findFile(FileManager::Config_Abilities, id));

	ability->setName(id);
	setValues(ability, parser.rootChild("Properties"));
	setRangedValues(ability);
	initAnimations(ability, parser.rootChild("Animator"));

	return ability;
}


void AbilityBuilder::initAnimations(Ability* ability, const XMLNode animationNode)
{
	AnimationReader reader;
	ability->setAnimations(reader.buildAnimator(animationNode, mTextures));
}


void AbilityBuilder::setValues(Ability* ability, const XMLNode propertiesNode)
{
	PropertyMap properties(propertiesNode);

	if (properties.isEmpty())
		DebugPrint(Log, "Ability '%s' has no property values\n", ability->name().c_str());

	ability->fillBaseValues(properties);
	ability->fillValues(properties);
}


Ability* AbilityBuilder::createNewAbility(const BasicString& id)
{
	Ability* ability = nullptr;
	AbilityType type = AbilityType::None;
	type << id;

	switch (type)
	{
		break;
	case AbilityType::Heal:
		ability = new HealAbility;
		break;
	case AbilityType::None:
	default:
		DebugPrint(Log, "Ability type for '%s' has not been set, must be added to the AbilityType enum\n", id.c_str());
		break;
	}

	return ability;
}

/*
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
		Texture* texture = textureManager->getTexture("Mjolnir", FileManager::Image_Weapons);
		VectorF size = realiseSize(texture->originalDimentions, values.get("HammerMaxSize"));
		ability = new SmashAbility(texture, size);
	}
	else if (name == "Charge")
	{
		ability = new ChargeAbility;
	}

*/

void AbilityBuilder::setRangedValues(Ability* ability)
{
	if (ability->isRanged())
	{
		RangedAbility* rangedAbility = static_cast<RangedAbility*>(ability);
		if (rangedAbility)
		{
			Texture* rangeCircle = mTextures->getTexture("RangeCircle", FileManager::Image_UI);
			rangedAbility->setRangeCircle(rangeCircle);
		}
	}
}