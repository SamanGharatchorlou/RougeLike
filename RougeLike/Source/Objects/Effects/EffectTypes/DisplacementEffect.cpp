#include "pch.h"
#include "DisplacementEffect.h"

#include "Actors/Actor.h"
#include "Map/Map.h"


DisplacementEffect::DisplacementEffect() : mDistance(0.0f), mForce(0.0f), mDistanceTravelled(0.0f) { }


void DisplacementEffect::set(VectorF source, float force, float distance)
{
	mSource = source;
	mForce = force;
	mDistance = distance;
}


void DisplacementEffect::fastUpdate(float dt)
{
	VectorF direction = (mReceiver->position() - mSource).normalise();
	VectorF velocity = direction * mForce;

	if (mDistanceTravelled >= mDistance)
	{
		endEffect();
	}
	else if (canMove(mReceiver, velocity, dt))
	{
		mReceiver->physics()->move(velocity, dt);

		float movementStep = std::sqrt(velocity.magnitudeSquared());
		mDistanceTravelled += movementStep * dt;
	}
}


void DisplacementEffect::clearData()
{
	clearBaseData();

	mSource = VectorF();
	mDistance = 0.0f;
	mForce = 0.0f;
	mDistanceTravelled = 0.0f;
}


// --- Private Functions --- //
//bool DisplacementEffect::canMove(VectorF velocity, float dt) const
//{
//	Index index;
//	const Map* map = mReceiver->currentMap();
//	RectF rect = mReceiver->scaledRect().Translate(velocity * dt);
//
//	index = map->index(rect.TopLeft());
//	if (!map->isValidIndex(index) || map->collisionType(index) == CollisionTile::Wall)
//		return false;
//
//	index = map->index(rect.TopRight());
//	if (!map->isValidIndex(index) || map->collisionType(index) == CollisionTile::Wall)
//		return false;
//
//	index = map->index(rect.BotRight());
//	if (!map->isValidIndex(index) || map->collisionType(index) == CollisionTile::Wall)
//		return false;
//
//	index = map->index(rect.BotLeft());
//	if (!map->isValidIndex(index) || map->collisionType(index) == CollisionTile::Wall)
//		return false;
//
//	return true;
//}
