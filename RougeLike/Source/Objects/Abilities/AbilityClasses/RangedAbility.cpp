#include "pch.h"

#include "RangedAbility.h"

#include "Game/Camera/Camera.h"
#include "Map/Map.h"

#include "Actors/Actor.h"

#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"
#include "Graphics/RenderManager.h"



void RangedAbility::init(Actor* caster, const PropertyMap& properties, Animator animator)
{
	Ability::init(caster, properties, animator);
	
	Texture* rangeCircle = TextureManager::Get()->getTexture("RangeCircle", FileManager::Image_UI);
	mRangeCircle = rangeCircle;
}


void RangedAbility::renderRangeCircle()
{
	VectorF position = Camera::Get()->toCameraCoords(mCaster->position());

	float range = mProperties[PropertyType::Range];
	VectorF size = VectorF(range, range) * 2.0f;

	RectF rect = RectF(VectorF(), size);
	rect.SetCenter(position);

	RenderEvent* event = new RenderEvent(mRangeCircle, rect, (int)RenderLayer::Floor);
	EventPacket ep(event);
	mEvents.push(ep);
}


bool RangedAbility::isValidTarget(VectorF target, const Map* map) const 
{
	if (mOnlyDirectional)
		return true;

	// Is it in range
	float range = mProperties.at(PropertyType::Range);
	if (distanceSquared(mCaster->position(), target) > range * range)
		return false;

	// Is the target position a floor tile
	RectF rect = mRect.MoveCopy(target);
	VectorF points[4]{ rect.TopLeft(), rect.TopRight(), rect.BotRight(), rect.BotLeft() };

	bool validPoint = true;
	for (int i = 0; i < 4; i++)
	{
		// IMPROVEMENT
		// if tile == nullptr that means we've probably asked for a tile on a different map
		// which could still be valid... but requires a pointer to that map
		const MapTile* tile = map->tile(points[i]);
		if (!tile || !map->tile(points[i])->is(CollisionTile::Floor))
			validPoint = false;
	}

	return validPoint;
}