#include "pch.h"
#include "KnockbackEffect.h"

#include "Objects/Actors/Actor.h"
#include "Game/GameData.h"
#include "Collisions/DamageCollider.h"
#include "Map/Environment.h"
#include "Map/Map.h"


KnockbackEffect::KnockbackEffect(const DamageCollider* sourceCollider)
{
	mSource = sourceCollider->rect().Center();
	mForce = sourceCollider->knockbackforce();
}


KnockbackEffect::KnockbackEffect(VectorF source, float force) : mSource(source), mForce(force) { }


void KnockbackEffect::init()
{
	timer.restart();
}


void KnockbackEffect::fastUpdate(float dt)
{
	VectorF direction = mActor->physics()->position() - mSource;
	VectorF velocity = direction.normalise() * mForce;

	if (canMove(velocity, dt))
		mActor->physics()->move(velocity, dt);
}


void KnockbackEffect::slowUpdate(float dt)
{
	printf("time: %f\n", timer.getSeconds());
	if (timer.getSeconds() > 0.1f)
	{
		printf("end effect\n");
		endEffect();
	}

}


/// --- Private Functions --- ///
bool KnockbackEffect::canMove(VectorF velocity, float dt) const
{
	Index index;
	const Map* map = mActor->getGameData()->environment->primaryMap();
	RectF rect = mActor->rect().Translate(velocity * dt);

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