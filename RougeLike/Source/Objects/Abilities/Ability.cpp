#include "pch.h"
#include "Ability.h"

#include "Map/Map.h"

EventPacket Ability::popEvent()
{
	ASSERT(Error, mEvents.size() > 0, "Ability has no event when attempting to pop one.\n");
	EventPacket event = mEvents.front();
	mEvents.pop();
	return event;
}


Collider AreaAbility::collider()
{
	return Collider(&mRect);
}


bool AreaAbility::isValidTarget(VectorF target)
{
	RectF rect = mRect.MoveCopy(target);
	VectorF points[4]{ rect.TopLeft(), rect.TopRight(), rect.BotRight(), rect.BotLeft() };

	bool validBlinkPoint = true;

	for (int i = 0; i < 4; i++)
	{
		if (!mMap->floorCollisionTile(points[i]))
		{
			validBlinkPoint = false;
		}
	}

	return validBlinkPoint;
}