#include "pch.h"
#include "AbilityBuilder.h"

#include "Graphics/TextureManager.h"
#include "Animations/AnimationReader.h"

// Abilities
#include "Objects/Abilities/AbilityTypes/Ability.h"
#include "Objects/Abilities/AbilityTypes/SlowAbility.h"
#include "Objects/Abilities/AbilityTypes/HealAbility.h"
#include "Objects/Abilities/AbilityTypes/SpikeAbility.h"
#include "Objects/Abilities/AbilityTypes/BilnkAbility.h"
#include "Objects/Abilities/AbilityTypes/ArmorAbility.h"
#include "Objects/Abilities/AbilityTypes/SmashAbility.h"
#include "Objects/Abilities/AbilityTypes/ChargeAbility.h"


Ability* AbilityBuilder::build(const std::string& id, TextureManager* textures)
{
	Ability* ability = createNewAbility(id);

	XMLParser parser;
	parser.parseXML(FileManager::Get()->findFile(FileManager::Config_Abilities, id));

	setValues(ability, id, parser);
	setRangedValues(ability, textures);
	initAnimations(ability, parser, textures);

	return ability;
}


void AbilityBuilder::initAnimations(Ability* ability, const XMLParser& parser, TextureManager* textures)
{
	Animator animator;
	AnimationReader reader(textures, parser);

	if (reader.initAnimator(animator))
		ability->setAnimations(animator);
	else
		DebugPrint(Log, "Ability animator setup failed\n");
}


void AbilityBuilder::setValues(Ability* ability, const std::string& id, const XMLParser& parser)
{
	ValueBag values;
	values.readData(parser, "Properties");

	if (values.isEmpty())
		DebugPrint(Log, "Ability '%s' has no property values\n", id.c_str());

	ability->fillAbilityValues(values);
	ability->fillValues(values);
	ability->setName(id);
}


Ability* AbilityBuilder::createNewAbility(const std::string& id)
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

	//if (id == "Heal")
	//{
	//	ability = new HealAbility;
	//}
	//else if (id == "Spikes")
	//{
	//	ability = new SpikeAbility;
	//}
	//else if (id == "Blink")
	//{
	//	ability = new BlinkAbility;
	//}
	//else if (id == "Armor")
	//{
	//	ability = new ArmorAbility;
	//}
	//else if (id == "Smash")
	//{
	//	//// Create hammer
	//	//Texture* texture = textureManager->getTexture("Mjolnir", FileManager::Image_Weapons);
	//	//VectorF size = realiseSize(texture->originalDimentions, values.get("HammerMaxSize"));
	//	//ability = new SmashAbility(texture, size);
	//	DebugPrint(Log, "Unimplemented '%s' ability\n", id.c_str());
	//}
	//else if (id == "Charge")
	//{
	//	ability = new ChargeAbility;
	//}

	if (!ability)
		DebugPrint(Warning, "No new ability could be created wth the name '%s'\n", id.c_str());

	return ability;
}



void AbilityBuilder::setRangedValues(Ability* ability, TextureManager* textures)
{
	if (ability->isRanged())
	{
		RangedAbility* rangedAbility = static_cast<RangedAbility*>(ability);
		if (rangedAbility)
		{
			Texture* rangeCircle = textures->getTexture("RangeCircle", FileManager::Image_UI);
			rangedAbility->setRangeCircle(rangeCircle);
		}
	}
}