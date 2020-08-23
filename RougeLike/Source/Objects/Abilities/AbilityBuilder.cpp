#include "pch.h"
#include "AbilityBuilder.h"

#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"
#include "Animations/AnimationReader.h"

// Abilities
#include "AbilityClasses/AbilityStates.h"
#include "Objects/Abilities/AbilityTypes/SlowAbility.h"
#include "Objects/Abilities/AbilityTypes/HealAbility.h"
#include "Objects/Abilities/AbilityTypes/SpikeAbility.h"
#include "Objects/Abilities/AbilityTypes/BilnkAbility.h"
#include "Objects/Abilities/AbilityTypes/ArmorAbility.h"
#include "Objects/Abilities/AbilityTypes/SmashAbility.h"
#include "Objects/Abilities/AbilityTypes/ChargeAbility.h"
#include "Objects/Abilities/AbilityTypes/SlashAbility.h"


Ability* AbilityBuilder::build(AbilityType type) const
{
	Ability* ability = createNewAbility(type);

	BasicString id;
	type >> id;
	XMLParser parser(FileManager::Get()->findFile(FileManager::Config_Abilities, id));

	AnimationReader reader;
	Animator animator = reader.buildAnimator(parser.rootChild("Animator"));

	PropertyMap properties(parser.rootChild("Properties"));

	ability->init(mCaster, properties, animator);

	return ability;
}


Ability* AbilityBuilder::createNewAbility(AbilityType type) const
{
	Ability* ability = nullptr;

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
		BasicString id;
		type >> id;

		XMLParser parser(FileManager::Get()->findFile(FileManager::Config_Abilities, id));
		XMLNode hammerDetailsNode = parser.rootChild("Hammer");
		StringMap hammerDetails(hammerDetailsNode);

		Texture* texture = TextureManager::Get()->getTexture(hammerDetails.at("Texture"), FileManager::Image_Weapons);
		VectorF size = realiseSize(texture->originalDimentions, hammerDetails.getFloat("MaxSize"));
		RectF rect(VectorF(), size);

		ability = new SmashAbility(texture, rect);
		break;
	}
	case AbilityType::Charge:
	{
		ability = new ChargeAbility;
		break;
	}
	case AbilityType::Slash:
	{
		ability = new SlashAbility;
		break;
	}

	case AbilityType::None:
	default:
	{
		BasicString id;
		type >> id;
		DebugPrint(Log, "Ability creation for type for '%s' has not been set, define in '%s'\n", id.c_str(), __FUNCTION__);
		break;
	}

	}

	return ability;
}

