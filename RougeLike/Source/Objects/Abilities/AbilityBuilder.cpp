#include "pch.h"
#include "AbilityBuilder.h"

#include "Graphics/Texture.h"
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


Ability* AbilityBuilder::build(const BasicString& id) const
{
	Ability* ability = createNewAbility(id);

	XMLParser parser(FileManager::Get()->findFile(FileManager::Config_Abilities, id));

	AnimationReader reader;
	Animator animator = reader.buildAnimator(parser.rootChild("Animator"), mTextures);

	PropertyMap properties(parser.rootChild("Properties"));

	ability->init(id, mCaster, properties, animator);
	setRangedValues(ability);

	return ability;
}


Ability* AbilityBuilder::createNewAbility(const BasicString& id) const
{
	Ability* ability = nullptr;
	AbilityType type = AbilityType::None;
	type << id;

	switch (type)
	{
	case AbilityType::Heal:
		ability = new HealAbility;
		break;

	case AbilityType::Blink:
		ability = new BlinkAbility;
		break;

	case AbilityType::Spikes:
		ability = new SpikeAbility;
		break;
	case AbilityType::Smash:
	{
		XMLParser parser(FileManager::Get()->findFile(FileManager::Config_Abilities, id));
		XMLNode hammerDetailsNode = parser.rootChild("Hammer");
		StringMap hammerDetails(hammerDetailsNode);

		Texture* texture = mTextures->getTexture(hammerDetails.at("Texture"), FileManager::Image_Weapons);
		VectorF size = realiseSize(texture->originalDimentions, hammerDetails.getFloat("MaxSize"));
		RectF rect(VectorF(), size);

		ability = new SmashAbility(texture, rect);
		break;
	}

	case AbilityType::None:
	default:
		DebugPrint(Log, "Ability type for '%s' has not been set, must be added to the AbilityType enum\n", id.c_str());
		break;
	}

	return ability;
}


void AbilityBuilder::setRangedValues(Ability* ability) const
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