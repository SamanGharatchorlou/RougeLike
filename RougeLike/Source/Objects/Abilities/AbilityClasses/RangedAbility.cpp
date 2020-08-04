#include "pch.h"
#include "RangedAbility.h"

#include "Game/Camera.h"
#include "Map/Map.h"

#include "Actors/Actor.h"
#include "Objects/Properties/PropertyBag.h"

#include "Graphics/Texture.h"
#include "Graphics/RenderManager.h"


// --- Range ---
void RangedAbility::fillAbilityValues(const ValueBag& values)
{
	Ability::fillAbilityValues(values);
	mRange = values.get("Range");
}


void RangedAbility::renderRangeCircle()
{
	VectorF position = Camera::Get()->toCameraCoords(mCaster->position());
	VectorF size = VectorF(mRange, mRange) * 2.0f;

	RectF rect = RectF(VectorF(), size);
	rect.SetCenter(position);

	mRangeCircle->render(rect);

	RenderEvent* event = new RenderEvent(mRangeCircle, rect, (int)RenderLayer::Floor);
	EventPacket ep(event);
	mEvents.push(ep);
}


Collider RangedAbility::collider()
{
	return Collider(&mRect);
}


bool RangedAbility::isValidTarget(VectorF target, const Map* map)
{
	// Is it in range
	if (distanceSquared(mCaster->position(), target) > mRange * mRange)
		return false;

	// Is the target position a floor tile
	RectF rect = mRect.MoveCopy(target);
	VectorF points[4]{ rect.TopLeft(), rect.TopRight(), rect.BotRight(), rect.BotLeft() };

	bool validPoint = true;
	for (int i = 0; i < 4; i++)
	{
		if (!map->tile(points[i])->is(CollisionTile::Floor))
			validPoint = false;
	}

	return validPoint;
}