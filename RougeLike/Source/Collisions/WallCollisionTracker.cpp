#include "pch.h"
#include "WallCollisionTracker.h"

#include "Map/Map.h"
#include "Actors/Actor.h"


VectorF WallCollisionTracker::allowedVelocity(const Map* map, VectorF velocity, float dt) const
{
	// extra buffer to stop small movemets getting stuck in the wall?
	float bufferFactor = 3.0f;
	velocity *= bufferFactor;

	restrictLeftMovement(map, velocity, dt);
	restrictRightMovement(map, velocity, dt);
	restrictDownMovement(map, velocity, dt);
	restrictTopMovement(map, velocity, dt);

	return velocity / bufferFactor;
}


void WallCollisionTracker::restrictLeftMovement(const Map* map, VectorF& movement, float dt) const
{
	if (movement.x < 0.0f)
	{
		RectF rect = wallScaledRect();
		rect = rect.Translate(VectorF(movement.x, 0.0f) * dt);

		const MapTile* topLeft = map->tile(rect.TopLeft());
		const MapTile* bottomLeft = map->tile(rect.BotLeft());

		if (cannotMove(topLeft) || cannotMove(bottomLeft))
			movement.x = 0.0f;
	}
}

void WallCollisionTracker::restrictRightMovement(const Map* map, VectorF& movement, float dt) const
{
	if (movement.x > 0.0f)
	{
		RectF rect = wallScaledRect();
		rect = rect.Translate(VectorF(movement.x, 0.0f) * dt);

		const MapTile* topRight = map->tile(rect.TopRight());
		const MapTile* bottomRight = map->tile(rect.BotRight());

		if (cannotMove(topRight) || cannotMove(bottomRight))
			movement.x = 0.0f;
	}
}

void WallCollisionTracker::restrictDownMovement(const Map* map, VectorF& movement, float dt) const
{
	if (movement.y > 0.0f)
	{
		RectF rect = wallScaledRect();
		rect = rect.Translate(VectorF(0.0f, movement.y) * dt);

		const MapTile* bottomRight = map->tile(rect.BotRight());
		const MapTile* bottomLeft = map->tile(rect.BotLeft());

		if (cannotMove(bottomRight) || cannotMove(bottomLeft))
			movement.y = 0.0f;
	}
}

void WallCollisionTracker::restrictTopMovement(const Map* map, VectorF& movement, float dt) const
{
	if (movement.y < 0.0f)
	{
		RectF rect = wallScaledRect();
		rect = rect.Translate(VectorF(0.0f, movement.y) * dt);

		const MapTile* topLeft = map->tile(rect.TopLeft());
		const MapTile* topRight = map->tile(rect.TopRight());

		if (cannotMove(topLeft) || cannotMove(topRight))
			movement.y = 0.0f;
	}
}


RectF WallCollisionTracker::wallScaledRect() const
{
	RectF rect;

	if (mActor)
		rect = mActor->scaledRect();
	else if(mRect)
		rect = *mRect;
	else
		DebugPrint(Error, "No rect or actor has been set to check wall collisions for, call setActor or setRect first\n");

	VectorF size = rect.Size();
	VectorF botCenter = rect.BotCenter();

	rect.SetSize(VectorF(size.x * 0.7f, size.y * 0.15f));
	rect.SetBotCenter(botCenter);
	return rect;
}




bool WallCollisionTracker::cannotMove(const MapTile* tile) const
{
	return tile && !tile->is(CollisionTile::Floor);
}

