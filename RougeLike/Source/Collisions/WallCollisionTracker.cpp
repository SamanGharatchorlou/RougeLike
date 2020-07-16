#include "pch.h"
#include "WallCollisionTracker.h"

#include "Map/Map.h"
#include "Actors/Actor.h"


void WallCollisionTracker::resolveWallCollisions(const Map* map, float dt)
{
	mActor->physics()->restrictMovement(Physics::Left, restrictLeftMovement(map, dt));
	mActor->physics()->restrictMovement(Physics::Right, restrictRightMovement(map, dt));
	mActor->physics()->restrictMovement(Physics::Up, restrictTopMovement(map, dt));
	mActor->physics()->restrictMovement(Physics::Down, restrictDownMovement(map, dt));
}


bool WallCollisionTracker::restrictLeftMovement(const Map* map, float dt) const
{
	float movement = mActor->physics()->maxMovementDistance(dt);
	RectF rect = wallScaledRect(VectorF(-movement, 0.0f));

	const MapTile* topLeft = map->tile(rect.TopLeft());
	const MapTile* bottomLeft = map->tile(rect.BotLeft());

	return cannotMove(topLeft) || cannotMove(bottomLeft);
}

bool WallCollisionTracker::restrictRightMovement(const Map* map, float dt) const
{
	float movement = mActor->physics()->maxMovementDistance(dt);
	RectF rect = wallScaledRect(VectorF(movement, 0.0f));

	const MapTile* topRight = map->tile(rect.TopRight());
	const MapTile* bottomRight = map->tile(rect.BotRight());

	return cannotMove(topRight) || cannotMove(bottomRight);
}

bool WallCollisionTracker::restrictDownMovement(const Map* map, float dt) const
{
	float movement = mActor->physics()->maxMovementDistance(dt);
	RectF rect = wallScaledRect(VectorF(0.0f, movement));

	const MapTile* bottomRight = map->tile(rect.BotRight());
	const MapTile* bottomLeft = map->tile(rect.BotLeft());

	return cannotMove(bottomRight) || cannotMove(bottomLeft);
}

bool WallCollisionTracker::restrictTopMovement(const Map* map, float dt) const
{
	float movement = mActor->physics()->maxMovementDistance(dt);
	RectF rect = wallScaledRect(VectorF(0.0f, -movement));

	const MapTile* topLeft = map->tile(rect.TopLeft());
	const MapTile* topRight = map->tile(rect.TopRight());

	return cannotMove(topLeft) || cannotMove(topRight);
}


RectF WallCollisionTracker::wallScaledRect(VectorF translation) const
{
	RectF rect = mActor->scaledRect();
	VectorF size = rect.Size();
	VectorF botCenter = rect.BotCenter();

	rect.SetSize(VectorF(size.x * 0.7f, size.y * 0.15f));
	rect.SetBotCenter(botCenter);

	return rect.Translate(translation);
}



bool WallCollisionTracker::cannotMove(const MapTile* tile) const
{
	return tile && !tile->is(CollisionTile::Floor);
}