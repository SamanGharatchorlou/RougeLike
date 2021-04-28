#include "pch.h"

#include "RangedAbility.h"
#include "Input/InputManager.h"

#include "Actors/Actor.h"
#include "Game/Camera/Camera.h"
#include "Graphics/TextureManager.h"
#include "Graphics/RenderManager.h"

#include "Map/Map.h"


// --- Ranged Ability --- //
bool RangedAbility::initiate(const InputManager* input) const
{
	return input->isCursorReleased(Cursor::Left);
}


void RangedAbility::init()
{
	Texture* rangeCircle = TextureManager::Get()->getTexture("RangeCircle", FileManager::Image_UI);
	mRangeCircle = rangeCircle;

	mCollider = new Collider(&mRect);
}


EventPacket RangedAbility::renderRangeCircleEvent()
{
	VectorF position = Camera::Get()->toCameraCoords(mCaster->position());

	float range = mProperties[PropertyType::Range];
	VectorF size = VectorF(range, range) * 2.0f;

	RectF rect = RectF(VectorF(), size);
	rect.SetCenter(position);

	RenderEvent* event = new RenderEvent(mRangeCircle, rect, (int)RenderLayer::Floor);
	return EventPacket(event);
}


// --- Target Area Ranged Ability --- //
bool TargetAreaRangedAbility::activateOn(Actor* target, EffectPool* effectPool)
{
	if (mHitList.count(target) == 0)
	{
		applyEffects(target, effectPool);
		mHitList.insert(target);
		return true;
	}

	return false;
}


bool TargetAreaRangedAbility::isValidTarget(VectorF target, const Map* map) const
{
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


void TargetAreaRangedAbility::baseExit()
{
	Ability::baseExit();

	mHitList.clear();
	mActivateCollisions = false;
}
