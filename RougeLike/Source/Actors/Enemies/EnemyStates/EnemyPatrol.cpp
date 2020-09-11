#include "pch.h"
#include "EnemyPatrol.h"

#include "AI/Pathing/AIPathMap.h"
#include "Actors/Enemies/Enemy.h"



void EnemyPatrol::init()
{
	mEnemy->animator().selectAnimation(Animation::Walk);
	setPatrolPoint();
}


void EnemyPatrol::fastUpdate(float dt)
{
	mEnemy->accellerateTowards(mPositionTarget);
}


void EnemyPatrol::slowUpdate(float dt)
{
	if (hasReachedPositionTarget())
		mEnemy->idle(1.0f);

	if (canSeeAttackTarget())
		mEnemy->replaceState(EnemyState::Alert);
}


void EnemyPatrol::render()
{
	mEnemy->renderCharacter();
}


void EnemyPatrol::resume()
{
	mEnemy->animator().selectAnimation(Animation::Walk);
	setPatrolPoint();
}


// --- Private Functions -- //

void EnemyPatrol::setPatrolPoint()
{
	const AIPathMap* map = mEnemy->pathing()->map();

	VectorF position = mEnemy->position();
	Index index = map->index(position);

	if (index.isPositive())
	{
		Vector2D<int> yTileRange = map->yTileFloorRange(index);

		if (!map->isValidIndex(Index(index.x, yTileRange.x)) ||
			!map->isValidIndex(Index(index.x, yTileRange.y)))
		{
			mPositionTarget = map->randomFloorTile()->rect().Center();
			DebugPrint(Warning, "The yTile range of %d,%d is not valid ofr x = %d. Getting a random path tile instead for enemy patrol\n", yTileRange.x, yTileRange.y, index.x);
		}

		VectorF highestPoint = map->tile(Index(index.x, yTileRange.x))->rect().BotCenter();
		VectorF lowestPoint = map->tile(Index(index.x, yTileRange.y))->rect().TopCenter();

		// set the furthest point
		VectorF patrolTarget = (position.y - highestPoint.y < lowestPoint.y - position.y) ? lowestPoint : highestPoint;
		ASSERT(Warning, map->isValidPosition(patrolTarget), "Invalid enemy patrol target: %f, %f", patrolTarget.x, patrolTarget.y);

		const PathTile* tile = map->tile(patrolTarget);
		mPositionTarget = tile->rect().Center();
	}
}



bool EnemyPatrol::canSeeAttackTarget() const
{
	if (mEnemy->hasTarget())
	{
		VectorF position = mEnemy->position();
		VectorF target = mEnemy->target()->position();

		return distance(target, position) < mEnemy->getAttributeValue(AttributeType::SightRange);
	}
	else
		return false;
}


bool EnemyPatrol::hasReachedPositionTarget() const
{
	VectorF position = mEnemy->position();
	return distanceSquared(position, mPositionTarget) < 10.0f;
}
