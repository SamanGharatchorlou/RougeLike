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


// Allow <-> movement if tiles == nullptr
void WallCollisionTracker::testLeftCollisions(const Map* map, float dt)
{
	float movement = mActor->physics()->maxMovementDistance(dt);
	RectF rect = wallScaledRect(VectorF(-movement, 0.0f));

	const MapTile* topLeft = map->tile(rect.TopLeft());
	const MapTile* bottomLeft = map->tile(rect.BotLeft());

	if ((topLeft && bottomLeft) &&
		(topLeft->is(CollisionTile::Wall) || bottomLeft->is(CollisionTile::Wall)))
	{
		mActor->physics()->restrictMovement(Physics::Left, true);
	}
}


// Allow <-> movement if tiles == nullptr
void WallCollisionTracker::testRightCollisions(const Map* map, float dt)
{
	float movement = mActor->physics()->maxMovementDistance(dt);
	RectF rect = wallScaledRect(VectorF(movement, 0.0f));

	const MapTile* topRight = map->tile(rect.TopRight());
	const MapTile* bottomRight = map->tile(rect.BotRight());

	if ((topRight && bottomRight) &&
		(topRight->is(CollisionTile::Wall) || bottomRight->is(CollisionTile::Wall)))
	{
		mActor->physics()->restrictMovement(Physics::Right, true);
	}
}


// Prevent ^v movement if tiles == nullptr
void WallCollisionTracker::testBottomCollisions(const Map* map, float dt)
{
	float movement = mActor->physics()->maxMovementDistance(dt);
	RectF rect = wallScaledRect(VectorF(0.0f, movement));

	const MapTile* bottomRight = map->tile(rect.BotRight());
	const MapTile* bottomLeft = map->tile(rect.BotLeft());

	if (!bottomRight || !bottomLeft ||
		bottomRight->is(CollisionTile::Wall) || bottomLeft->is(CollisionTile::Wall))
	{
		mActor->physics()->restrictMovement(Physics::Down, true);
	}
}


// Prevent ^v movement if tiles == nullptr
void WallCollisionTracker::testTopCollisions(const Map* map, float dt)
{
	float movement = mActor->physics()->maxMovementDistance(dt);
	RectF rect = wallScaledRect(VectorF(0.0f, -movement));

	const MapTile* topLeft = map->tile(rect.TopLeft());
	const MapTile* topRight = map->tile(rect.TopRight());

	if (!topLeft || !topRight ||
		topLeft->is(CollisionTile::Wall) || topRight->is(CollisionTile::Wall))
	{
		mActor->physics()->restrictMovement(Physics::Up, true);
	}
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