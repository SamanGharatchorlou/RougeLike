#include "pch.h"
#include "Ability.h"

#include "Graphics/Texture.h"
#include "Game/Camera.h"
#include "Map/Map.h"
#include "Objects/Actors/Player/Player.h"

#include "Objects/Abilities/SlowAbility.h"
#include "Objects/Abilities/HealAbility.h"
#include "Objects/Abilities/SpikeAbility.h"
#include "Objects/Abilities/BilnkAbility.h"
#include "Objects/Abilities/ArmorAbility.h"
#include "Objects/Abilities/SmashAbility.h"

#include "Debug/DebugDraw.h"


EventPacket Ability::popEvent()
{
	ASSERT(Error, mEvents.size() > 0, "Ability has no event when attempting to pop one.\n");
	EventPacket event = mEvents.front();
	mEvents.pop();
	return event;
}

void Ability::init(Animator animator, Player* player)
{
	mPlayer = player;
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
	if (mState == Running)
	{
		RectF rect = Camera::Get()->toCameraCoords(mRect);
		mAnimator.getSpriteTile()->render(rect);
	}
}


void AreaAbility::render()
{
	// Range circle
	if (mState == Selected)
	{
		VectorF position = Camera::Get()->toCameraCoords(mPlayer->position());
		VectorF size = VectorF(mRange, mRange) * 2.0f;

		RectF rect = RectF(VectorF(), size);
		rect.SetCenter(position);

		mRangeCircle->render(rect);
	}

	Ability::render();
}



Collider AreaAbility::collider()
{
	return Collider(&mRect);
}


bool AreaAbility::isValidTarget(VectorF target, Map* map)
{
	// Is it in range
	if (distanceSquared(mPlayer->position(), target) > mRange * mRange)
		return false;

	// Is the target position a floor tile
	RectF rect = mRect.MoveCopy(target);
	VectorF points[4]{ rect.TopLeft(), rect.TopRight(), rect.BotRight(), rect.BotLeft() };

	bool validPoint = true;

	for (int i = 0; i < 4; i++)
	{
		if (!map->floorCollisionTile(points[i]))
		{
			validPoint = false;
		}
	}

	return validPoint;
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