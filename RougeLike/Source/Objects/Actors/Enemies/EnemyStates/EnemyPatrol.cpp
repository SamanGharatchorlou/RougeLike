#include "pch.h"
#include "EnemyPatrol.h"

#include "Map/Map.h"
#include "Objects/Actors/Enemies/Enemy.h"



EnemyPatrol::EnemyPatrol(Enemy* enemy) : EnemyState(enemy) { }


void EnemyPatrol::init()
{
	mEnemy->animator().selectAnimation(Action::Walk);
	setPatrolPoint();
}


void EnemyPatrol::fastUpdate(float dt)
{
	mEnemy->accellerateTowards(mPositionTarget);
}


void EnemyPatrol::slowUpdate(float dt)
{
	mEnemy->resolveCollisions();

	// TODO: Set idle for a time?
	// Enemy cannot be hit while in the wait state
	if (hasReachedPositionTarget())
		mEnemy->addIdleState(1.0f);

	if (canSeeAttackTarget())
		mEnemy->replaceState(EnemyState::Alert);
}


void EnemyPatrol::render()
{
	mEnemy->renderCharacter();
}


void EnemyPatrol::resume()
{
	mEnemy->animator().selectAnimation(Action::Walk);
	setPatrolPoint();
}


// --- Private Functions -- //

void EnemyPatrol::setPatrolPoint()
{
	const Map* map = mEnemy->getEnvironmentMap();

	VectorF position = mEnemy->position();
	Index index = map->index(position);

	Vector2D<int> yTileRange = map->yTileFloorRange(index.x);// findYFloorTileRange(tilePositionIndex.x, map);

	VectorF highestPoint = map->tile(Index(index.x, yTileRange.x))->rect().Center();
	VectorF lowestPoint = map->tile(Index(index.x, yTileRange.y))->rect().Center();

	// set the furthest point
	VectorF patrolTarget = (position.y - highestPoint.y < lowestPoint.y - position.y) ? lowestPoint : highestPoint;
	ASSERT(Warning, map->isValidPosition(patrolTarget), "Invalid enemy patrol target: %f, %f", patrolTarget.x, patrolTarget.y);

	const MapTile* tile = map->tile(patrolTarget);
	mPositionTarget = tile->rect().Center();
}


Vector2D<int> EnemyPatrol::findYFloorTileRange(int xIndex, const Map* map) const
{
	int yTileIndex = 0;
	Vector2D<int> yTileRange;

	bool isWall = true;
	while (isWall)
	{
		Index index(xIndex, ++yTileIndex);
		isWall = map->tile(index)->is(CollisionTile::Wall);
	}

	// Top
	yTileRange.x = yTileIndex;

	while (!isWall)
	{
		if (yTileIndex == map->yCount() - 1)
			break;

		Index index(xIndex, ++yTileIndex);
		isWall = map->tile(index)->is(CollisionTile::Wall);
	}

	yTileRange.y = yTileIndex;
	return yTileRange;
}



bool EnemyPatrol::canSeeAttackTarget() const
{
	VectorF position = mEnemy->position();
	VectorF target = mEnemy->target()->position();

	return distanceSquared(target, position) < mEnemy->getPropertyValue("SightRange");
}


bool EnemyPatrol::hasReachedPositionTarget() const
{
	VectorF position = mEnemy->position();
	return distanceSquared(position, mPositionTarget) < 10.0f;
}
