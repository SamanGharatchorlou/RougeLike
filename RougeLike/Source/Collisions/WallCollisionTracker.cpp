#include "pch.h"
#include "WallCollisionTracker.h"

#include "Objects/Actors/Actor.h"
#include "Collider.h"

#include "Map/Map.h"

// Wall Collisions
void WallCollisionTracker::resolveWallCollisions(const Map* map, float dt)
{
	mActor->physics()->resetAllowedMovement();

	RectF collisionRect = mActor->collider()->scaledRect();

	bool restrictLeft = doesCollideLeft(map, collisionRect.TopLeft(), dt) ||
		doesCollideLeft(map, collisionRect.BotLeft(), dt);

	bool restrictRight = doesCollideRight(map, collisionRect.TopRight(), dt) ||
		doesCollideRight(map, collisionRect.BotRight(), dt);

	bool restrictUp = doesCollideTop(map, collisionRect.TopLeft(), dt) ||
		doesCollideTop(map, collisionRect.TopRight(), dt);

	bool restrictDown = doesCollideBot(map, collisionRect.BotLeft(), dt) ||
		doesCollideBot(map, collisionRect.BotRight(), dt);

	mActor->physics()->restrictMovement(Physics::Up, restrictUp);
	mActor->physics()->restrictMovement(Physics::Down, restrictDown);
	mActor->physics()->restrictMovement(Physics::Right, restrictRight);
	mActor->physics()->restrictMovement(Physics::Left, restrictLeft);
}


bool WallCollisionTracker::doesCollideLeft(const Map* map, const VectorF point, float dt) const
{
	if (!map->isValidPosition(point))
		return false;

	bool willCollide = false;
	const MapTile* currentTile = map->tile(point);

	if (currentTile)
	{
		ASSERT(Warning, currentTile->collisionType() == MapTile::Floor,
			"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
			currentTile->index.x, currentTile->index.y, currentTile->collisionType());

		const MapTile* leftTile = map->offsetTile(currentTile, -1, 0);

		if (leftTile && leftTile->hasCollisionType(MapTile::Right ^ MapTile::Wall))
		{
			float xFuturePosition = point.x - mActor->physics()->maxMovementDistance(dt);
			willCollide = xFuturePosition < leftTile->rect().RightPoint();
		}
	}

	return willCollide;
}


bool WallCollisionTracker::doesCollideRight(const Map* map, const VectorF point, float dt) const
{
	if (!map->isValidPosition(point))
		return false;

	bool willCollide = false;
	const MapTile* currentTile = map->tile(point);

	if (currentTile)
	{
		ASSERT(Warning, currentTile->collisionType() == MapTile::Floor,
			"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
			currentTile->index.x, currentTile->index.y, currentTile->collisionType());

		const MapTile* rightTile = map->offsetTile(currentTile, +1, 0);

		if (rightTile && rightTile->hasCollisionType(MapTile::Left ^ MapTile::Wall))
		{
			float xFuturePosition = point.x + mActor->physics()->maxMovementDistance(dt);
			willCollide = xFuturePosition > rightTile->rect().LeftPoint();
		}
	}

	return willCollide;
}


bool WallCollisionTracker::doesCollideTop(const Map* map, const VectorF point, float dt) const
{
	if (!map->isValidPosition(point))
		return false;

	bool willCollide = false;
	const MapTile* currentTile = map->tile(point);

	if (currentTile)
	{
		ASSERT(Warning, currentTile->collisionType() == MapTile::Floor,
			"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
			currentTile->index.x, currentTile->index.y, currentTile->collisionType());

		const MapTile* upTile = map->offsetTile(currentTile, 0, -1);

		if (upTile && upTile->hasCollisionType(MapTile::Bot))
		{
			float yFuturePosition = point.y - mActor->physics()->maxMovementDistance(dt);
			willCollide = yFuturePosition < upTile->rect().BotPoint();
		}
	}

	return willCollide;
}


bool WallCollisionTracker::doesCollideBot(const Map* map, const VectorF point, float dt) const
{
	if (!map->isValidPosition(point))
		return false;

	bool willCollide = false;
	const MapTile* currentTile = map->tile(point);

	if (currentTile)
	{
		ASSERT(Warning, currentTile->collisionType() == MapTile::Floor,
			"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
			currentTile->index.x, currentTile->index.y, currentTile->collisionType());

		const MapTile* downTile = map->offsetTile(currentTile, 0, +1);

		if (downTile && downTile->hasCollisionType(MapTile::Top))
		{
			float yFuturePosition = point.y + mActor->physics()->maxMovementDistance(dt);
			willCollide = yFuturePosition > downTile->rect().TopPoint();
		}
	}

	return willCollide;
}

