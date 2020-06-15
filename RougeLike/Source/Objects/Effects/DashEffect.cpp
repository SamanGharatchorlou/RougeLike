#include "pch.h"
#include "DashEffect.h"

#include "Objects/Actors/Actor.h"
#include "Map/Map.h"





void DashEffect::fastUpdate(float dt)
{
	float distanceToTarget = distanceSquared(mActor->position(), mTarget);

	VectorF direction = mActor->position();
	VectorF velocity = direction.normalise() * 1000.0f;

	float movementStep = distanceSquared(VectorF(), velocity);

	if (distanceToTarget < movementStep)
		endEffect();
	else if (canMove(velocity, dt))
		mActor->physics()->move(velocity, dt);
}



void DashEffect::slowUpdate(float dt)
{

}





// --- Private Functions --- //
bool DashEffect::canMove(VectorF velocity, float dt) const
{
	Index index;
	const Map* map = mActor->currentMap();
	RectF rect = mActor->scaledRect().Translate(velocity * dt);

	index = map->index(rect.TopLeft());
	if (!map->isValidIndex(index) || !map->floorCollisionTile(index))
		return false;

	index = map->index(rect.TopRight());
	if (!map->isValidIndex(index) || !map->floorCollisionTile(index))
		return false;

	index = map->index(rect.BotRight());
	if (!map->isValidIndex(index) || !map->floorCollisionTile(index))
		return false;

	index = map->index(rect.BotLeft());
	if (!map->isValidIndex(index) || !map->floorCollisionTile(index))
		return false;

	return true;
}