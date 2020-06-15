#include "pch.h"
#include "DisplacementEffect.h"

#include "Objects/Actors/Actor.h"
#include "Map/Map.h"


DisplacementEffect::DisplacementEffect(VectorF source, float distance, float force)
	: mSource(source), mDistance(distance), mForce(force), mDistanceTravelled(0.0f) { }


void DisplacementEffect::fastUpdate(float dt)
{
	VectorF direction = (mActor->position() - mSource).normalise();
	VectorF velocity = direction * mForce;
	float movementStep = velocity.magnitudeSquared();

	if (mDistanceTravelled >= mDistance)
	{
		endEffect();
	}
	else if (canMove(velocity, dt))
	{
		mActor->physics()->move(velocity, dt);
		mDistanceTravelled += movementStep * dt;
	}
}


// --- Private Functions --- //
bool DisplacementEffect::canMove(VectorF velocity, float dt) const
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
