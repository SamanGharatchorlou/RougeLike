#include "pch.h"
#include "WallCollisionTracker.h"

#include "Map/Map.h"
#include "Actors/Actor.h"


VectorF WallCollisionTracker::allowedVelocity(const Map* map, VectorF velocity, float dt) 
{
	if (braodPhaseCollision(map, velocity, dt))
	{
		mWallScaledRect = wallScaledRect();

		// Increase the movement to make sure we dont get stuck in a wall or something
		float bufferFactor = 2.0f;
		velocity *= bufferFactor;

		if (velocity.x > 0.0f)
		{
			restrictRightMovement(map, velocity, dt);
		}
		else if (velocity.x < 0.0f)
		{
			restrictLeftMovement(map, velocity, dt);
		}

		if (velocity.y > 0.0f)
		{
			restrictDownMovement(map, velocity, dt);
		}
		else if (velocity.y < 0.0f)
		{
			restrictUpMovement(map, velocity, dt);
		}

		return velocity / bufferFactor;
	}
	else // no collision so we dont touch the velocity
	{
		return velocity;
	}
}


bool WallCollisionTracker::braodPhaseCollision(const Map* map, VectorF& velocity, float dt) const
{
	// Create a rect with size = size + movement and check if any corners are on a wall tile
	RectF rect = wallScaledRect();
	VectorF center = rect.Center();
	VectorF size = rect.Size() + velocity * 2.0f * dt;
	rect.SetSize(size);
	rect.SetCenter(center);

	const MapTile* tiles[4] = { map->tile(rect.TopLeft()), map->tile(rect.TopRight()), map->tile(rect.BotRight()), map->tile(rect.BotLeft()) };
	for (int i = 0; i < 4; i++)
	{
		if (tiles[i] && tiles[i]->is(CollisionTile::Floor))
		{
			return true;
		}
	}

	return false;
}


void WallCollisionTracker::restrictUpMovement(const Map* map, VectorF& velocity, float dt) const
{
	RectF rect = mWallScaledRect;
	rect = rect.Translate(VectorF(0.0f, velocity.y) * dt);

	const MapTile* topLeft = map->tile(rect.TopLeft());
	const MapTile* topRight = map->tile(rect.TopRight());

	if (cannotMove(topLeft) || cannotMove(topRight))
		velocity.y = 0.0f;
}


void WallCollisionTracker::restrictRightMovement(const Map* map, VectorF& velocity, float dt) const
{
	RectF rect = mWallScaledRect;
	rect = rect.Translate(VectorF(velocity.x, 0.0f) * dt);

	const MapTile* topRight = map->tile(rect.TopRight());
	const MapTile* bottomRight = map->tile(rect.BotRight());

	if (cannotMove(topRight) || cannotMove(bottomRight))
		velocity.x = 0.0f;
}



void WallCollisionTracker::restrictDownMovement(const Map* map, VectorF& velocity, float dt) const
{
	RectF rect = mWallScaledRect;
	rect = rect.Translate(VectorF(0.0f, velocity.y) * dt);

	const MapTile* bottomRight = map->tile(rect.BotRight());
	const MapTile* bottomLeft = map->tile(rect.BotLeft());

	if (cannotMove(bottomRight) || cannotMove(bottomLeft))
		velocity.y = 0.0f;
}



void WallCollisionTracker::restrictLeftMovement(const Map* map, VectorF& velocity, float dt) const
{
	RectF rect = mWallScaledRect;
	rect = rect.Translate(VectorF(velocity.x, 0.0f) * dt);

	const MapTile* topLeft = map->tile(rect.TopLeft());
	const MapTile* bottomLeft = map->tile(rect.BotLeft());

	if (cannotMove(topLeft) || cannotMove(bottomLeft))
		velocity.x = 0.0f;
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

