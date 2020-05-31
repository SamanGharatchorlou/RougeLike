#include "pch.h"
#include "WallCollisionTracker.h"

#include "Objects/Actors/Actor.h"
#include "Collider.h"

#include "Map/Map.h"

// Wall Collisions
void WallCollisionTracker::resolveWallCollisions(const Map* map, float dt)
{
	mActor->physics()->resetAllowedMovement();

	testLeftCollisions(map, dt);
	testTopCollisions(map, dt);
	testRightCollisions(map, dt);
	testBottomCollisions(map, dt);
}


void WallCollisionTracker::testLeftCollisions(const Map* map, float dt)
{
	float movement = mActor->physics()->maxMovementDistance(dt);
	RectF rect = mActor->rect().Translate(VectorF(-movement, 0.0f));

	const MapTile* topLeft = map->tile(rect.TopLeft());
	const MapTile* bottomLeft = map->tile(rect.BotLeft());

	if (!topLeft || !bottomLeft ||
		topLeft->hasCollisionType(MapTile::Wall) || bottomLeft->hasCollisionType(MapTile::Wall))
	{
		mActor->physics()->restrictMovement(Physics::Left, true);
	}
}

void WallCollisionTracker::testTopCollisions(const Map* map, float dt)
{
	float movement = mActor->physics()->maxMovementDistance(dt);
	RectF rect = mActor->rect().Translate(VectorF(0.0f, -movement));

	const MapTile* topLeft = map->tile(rect.TopLeft());
	const MapTile* topRight = map->tile(rect.TopRight());

	if (!topLeft || !topRight ||
		topLeft->hasCollisionType(MapTile::Wall) || topRight->hasCollisionType(MapTile::Wall))
	{
		mActor->physics()->restrictMovement(Physics::Up, true);
	}
}

void WallCollisionTracker::testRightCollisions(const Map* map, float dt)
{
	float movement = mActor->physics()->maxMovementDistance(dt);
	RectF rect = mActor->rect().Translate(VectorF(movement, 0.0f));

	const MapTile* topRight = map->tile(rect.TopRight());
	const MapTile* bottomRight = map->tile(rect.BotRight());

	if (!topRight || !bottomRight ||
		topRight->hasCollisionType(MapTile::Wall) || bottomRight->hasCollisionType(MapTile::Wall))
	{
		mActor->physics()->restrictMovement(Physics::Right, true);
	}
}

void WallCollisionTracker::testBottomCollisions(const Map* map, float dt)
{
	float movement = mActor->physics()->maxMovementDistance(dt);
	RectF rect = mActor->rect().Translate(VectorF(0.0f, movement));

	const MapTile* bottomRight = map->tile(rect.BotRight());
	const MapTile* bottomLeft = map->tile(rect.BotLeft());

	if (!bottomRight || !bottomLeft ||
		bottomRight->hasCollisionType(MapTile::Wall) || bottomLeft->hasCollisionType(MapTile::Wall))
	{
		mActor->physics()->restrictMovement(Physics::Down, true);
	}
}
