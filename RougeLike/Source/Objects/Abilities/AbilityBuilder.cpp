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


Ability* AbilityBuilder::build(const std::string& id)
{
	Ability* ability = createNewAbility(id);

	XMLParser parser;
	parser.parseXML(FileManager::Get()->findFile(FileManager::Config_Abilities, id));

	setValues(ability, id, parser);
	setRangedValues(ability);
	initAnimations(ability, parser);

	return ability;
}


void AbilityBuilder::initAnimations(Ability* ability, const XMLParser& parser)
{
	Animator animator;
	AnimationReader reader(mTextures, parser);

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

	return ability;
}



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