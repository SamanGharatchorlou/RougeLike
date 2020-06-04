#include "pch.h"
#include "Ability.h"

#include "Game/Camera.h"
#include "Map/Map.h"

#include "Objects/Abilities/SlowAbility.h"
#include "Objects/Abilities/HealAbility.h"
#include "Objects/Abilities/SpikeAbility.h"
#include "Objects/Abilities/BilnkAbility.h"
#include "Objects/Abilities/ArmorAbility.h"
#include "Objects/Abilities/SmashAbility.h"


EventPacket Ability::popEvent()
{
	ASSERT(Error, mEvents.size() > 0, "Ability has no event when attempting to pop one.\n");
	EventPacket event = mEvents.front();
	mEvents.pop();
	return event;
}

void Ability::init(Animator animator)
{
	mAnimator = animator;
	realiseSize();
}


void Ability::realiseSize()
{
	ASSERT(Warning, mAnimator.hasAnimations(), "Must call Ability::init before realiseSize function\n");
	ASSERT(Warning, mMaxDimention != 0.0f, "Max dimentions have not been set\n");

	VectorF baseSize = mAnimator.getSpriteTile()->getRect().Size();
	VectorF ratio = baseSize / mMaxDimention;
	float maxRatio = std::max(ratio.x, ratio.y);

	mRect.SetSize(baseSize / maxRatio);
}


void Ability::render()
{
	RectF rect = Camera::Get()->toCameraCoords(mRect);
	mAnimator.getSpriteTile()->render(rect);
}


Collider AreaAbility::collider()
{
	return Collider(&mRect);
}


bool AreaAbility::isValidTarget(VectorF target, Map* map)
{
	RectF rect = mRect.MoveCopy(target);
	VectorF points[4]{ rect.TopLeft(), rect.TopRight(), rect.BotRight(), rect.BotLeft() };

	bool validBlinkPoint = true;

	for (int i = 0; i < 4; i++)
	{
		if (!map->floorCollisionTile(points[i]))
		{
			validBlinkPoint = false;
		}
	}

	return validBlinkPoint;
}


Ability* createNewAbility(const std::string& name)
{
	XMLParser parser;
	parser.parseXML(FileManager::Get()->findFile(FileManager::Config_Abilities, name));
	ValueMap values = parser.values(parser.rootNode());

	Ability* ability = nullptr;

	if (name == "Slow")
	{
		ability = new SlowAbility;
	}
	else if (name == "Heal")
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
		ability = new SmashAbility;
	}

	if (ability)
		ability->fillValues(values);
	else
		DebugPrint(Warning, "No new ability could be created wth the name '%s'\n", name.c_str());

	return ability;
}