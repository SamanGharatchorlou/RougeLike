#include "pch.h"
#include "DisplacementEffect.h"

#include "Objects/Actors/Actor.h"
#include "Map/Map.h"


DisplacementEffect::DisplacementEffect() : mDistance(0.0f), mForce(0.0f), mDistanceTravelled(0.0f) { }

DisplacementEffect::DisplacementEffect(VectorF source, float distance, float force)
	: mSource(source), mDistance(distance), mForce(force), mDistanceTravelled(0.0f) { }


void DisplacementEffect::fastUpdate(float dt)
{


	VectorF direction = (mReceiver->position() - mSource).normalise();
	VectorF velocity = direction * mForce;
	float movementStep = velocity.magnitudeSquared();

	if (mDistanceTravelled >= mDistance)
	{
		endEffect();
	}
	else if (canMove(velocity, dt))
	{
		mReceiver->physics()->move(velocity, dt);
		mDistanceTravelled += movementStep * dt;
	}
}

void DisplacementEffect::clearData()
{
	mSource = VectorF();
	mDistance = 0.0f;
	mForce = 0.0f;
	mDistanceTravelled = 0.0f;

	printf("clear %p data\n", this);

	Effect::clearData();
}


void DisplacementEffect::fillData(const Actor* distributer)
{
	mSource = distributer->position();

	if (hasProperty(distributer, "KnockbackDistance"))
		mDistance = distributer->getPropertyValue("KnockbackDistance");


	if (hasProperty(distributer, "KnockbackForce"))
		mForce = distributer->getPropertyValue("KnockbackForce");

	mDistanceTravelled = 0.0f;
}


// --- Private Functions --- //
bool DisplacementEffect::canMove(VectorF velocity, float dt) const
{
	Index index;
	const Map* map = mReceiver->currentMap();
	RectF rect = mReceiver->scaledRect().Translate(velocity * dt);

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
