#include "pch.h"
#include "WallCollisionTracker.h"

#include "Map/Map.h"
#include "Actors/Actor.h"



VectorF WallCollisionTracker::allowedMovement(const Map* map, VectorF movement)
{
	// extra buffer to stop small movemets getting stuck in the wall?
	//float bufferFactor = 3.0f;

	//movement = movement * bufferFactor;

	//restrictLeftMovement(map, movement);
	//restrictRightMovement(map, movement);
	//restrictDownMovement(map, movement);
	//restrictTopMovement(map, movement);

	//return movement / bufferFactor;
	return VectorF();
}

VectorF WallCollisionTracker::allowedVelocity(const Map* map, VectorF velocity, float dt)
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
	RectF rect = mActor->scaledRect();
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




// ----------------------------

//void WallCollisionTracker2::init(Actor* actor)
//{
//	mActor = actor;
//}
//
//void WallCollisionTracker2::resolveWallCollisions(const Map* map, VectorF& movement)
//{
//	VectorF moveMe = VectorF(std::abs(movement.x), std::abs(movement.y));
//	if (restrictLeftMovement(map, moveMe))
//	{
//		movement.x = clamp(movement.x, 0.0f, std::abs(movement.x));
//	}
//	if (restrictRightMovement(map, moveMe))
//	{
//		movement.x = clamp(movement.x, -std::abs(movement.x), 0.0f);
//	}
//	if (restrictTopMovement(map, moveMe))
//	{
//		movement.y = clamp(movement.y, 0.0f, std::abs(movement.y));
//	}
//	if (restrictDownMovement(map, moveMe))
//	{
//		movement.y = clamp(movement.y, -std::abs(movement.y), 0.0f);
//	}
//}
//
//
//bool WallCollisionTracker2::restrictLeftMovement(const Map* map, VectorF movement) const
//{
//	RectF rect = wallScaledRect(VectorF(-movement.x, 0.0f));
//
//	const MapTile* topLeft = map->tile(rect.TopLeft());
//	const MapTile* bottomLeft = map->tile(rect.BotLeft());
//
//	return cannotMove(topLeft) || cannotMove(bottomLeft);
//}
//
//bool WallCollisionTracker2::restrictRightMovement(const Map* map, VectorF movement) const
//{
//	RectF rect = wallScaledRect(VectorF(movement.x, 0.0f));
//
//	const MapTile* topRight = map->tile(rect.TopRight());
//	const MapTile* bottomRight = map->tile(rect.BotRight());
//
//	return cannotMove(topRight) || cannotMove(bottomRight);
//}
//
//bool WallCollisionTracker2::restrictDownMovement(const Map* map, VectorF movement) const
//{
//	RectF rect = wallScaledRect(VectorF(0.0f, movement.y));
//
//	const MapTile* bottomRight = map->tile(rect.BotRight());
//	const MapTile* bottomLeft = map->tile(rect.BotLeft());
//
//	return cannotMove(bottomRight) || cannotMove(bottomLeft);
//}
//
//bool WallCollisionTracker2::restrictTopMovement(const Map* map, VectorF movement) const
//{
//	RectF rect = wallScaledRect(VectorF(0.0f, -movement.y));
//
//	const MapTile* topLeft = map->tile(rect.TopLeft());
//	const MapTile* topRight = map->tile(rect.TopRight());
//
//	return cannotMove(topLeft) || cannotMove(topRight);
//}
//
//
//bool WallCollisionTracker2::cannotMove(const MapTile* tile) const
//{
//	return tile && !tile->is(CollisionTile::Floor);
//}
//
//
//RectF WallCollisionTracker2::wallScaledRect(VectorF translation) const
//{
//	RectF rect = mActor->scaledRect();
//	VectorF size = rect.Size();
//	VectorF botCenter = rect.BotCenter();
//
//	rect.SetSize(VectorF(size.x * 0.7f, size.y * 0.15f));
//	rect.SetBotCenter(botCenter);
//
//	return rect.Translate(translation);
//}