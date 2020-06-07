#include "pch.h"
#include "EnemyPatrol.h"

#include "EnemyIdle.h"
#include "../Enemy.h"

#include "Map/Map.h"


EnemyPatrol::EnemyPatrol(Enemy* enemy) : EnemyState(enemy) { }


void EnemyPatrol::init()
{
	mEnemy->animator().selectAnimation(Action::Run);
	setPatrolPoint();
}


void EnemyPatrol::fastUpdate(float dt)
{
	mEnemy->accellerateTowards(mEnemy->positionTargetRect()->Center());
}


void EnemyPatrol::slowUpdate(float dt)
{
	mEnemy->resolveCollisions();

	if (hasReachedPositionTarget())
		mEnemy->addState(EnemyState::Wait);

	if (canSeeAttackTarget())
		mEnemy->replaceState(EnemyState::Alert);
}


void EnemyPatrol::render()
{
	mEnemy->renderCharacter();
}


void EnemyPatrol::resume()
{
	mEnemy->animator().selectAnimation(Action::Run);
	setPatrolPoint();
}


// --- Private Functions -- //

void EnemyPatrol::setPatrolPoint()
{
	const Map* map = mEnemy->getEnvironmentMap();

	VectorF position = mEnemy->position();
	Index tilePositionIndex = map->index(position);

	Index yTileRange = findYFloorTileRange(tilePositionIndex.x);

	VectorF highestPoint = map->tile(Index(tilePositionIndex.x, yTileRange.x))->rect().Center();
	VectorF lowestPoint = map->tile(Index(tilePositionIndex.x, yTileRange.y))->rect().Center();

	// set the furthest point
	VectorF patrolTarget = (position.y - highestPoint.y < lowestPoint.y - position.y) ? lowestPoint : highestPoint;
	ASSERT(Warning, map->isValidPosition(patrolTarget), "Invalid enemy patrol target: %f, %f", patrolTarget.x, patrolTarget.y);

	const MapTile* tile = map->tile(patrolTarget);
	mEnemy->setPositionTarget(tile->rectPtr());
}


Vector2D<int> EnemyPatrol::findYFloorTileRange(int xIndex) const
{
	int yTileIndex = 0;
	Vector2D<int> yTileRange;
	const Map* map = mEnemy->getEnvironmentMap();

	while (map->wallCollisionTile(Index(xIndex, ++yTileIndex))) {}

	// Top
	yTileRange.x = yTileIndex;

	while (map->floorCollisionTile(Index(xIndex, ++yTileIndex)))
	{
		if (yTileIndex >= map->yCount() - 1)
			break;
	}

	yTileRange.y = clamp(yTileIndex - 2, yTileRange.x, (int)map->yCount() - 2);
	return yTileRange;
}



bool EnemyPatrol::canSeeAttackTarget() const
{
	VectorF position = mEnemy->position();
	VectorF attackTargetPosition = mEnemy->attackTargetRect()->Center();

	bool isNearby = distanceSquared(attackTargetPosition, position) < mEnemy->getPropertyValue("SightRange");
	bool hasLineOfSight = false;

	if (!isNearby)
	{
		// Is there a line of sight?
		float yPatrolDirection = (mEnemy->positionTargetRect()->Center() - position).y;
		float yTargetDirection = (attackTargetPosition - position).y;

		bool isFacingTarget = 
			(yPatrolDirection > 0.0f && yTargetDirection > 0.0f) ||
			(yPatrolDirection < 0.0f && yTargetDirection < 0.0f);

		if (isFacingTarget)
		{
			const Map* map = mEnemy->getEnvironmentMap();

			int enemyXTile = map->index(position).x;
			int playerXTile = -99;
			if (map->isValidPosition(attackTargetPosition))
			{
				playerXTile = map->index(attackTargetPosition).x;
			}
			hasLineOfSight = (enemyXTile == playerXTile) || (enemyXTile == playerXTile + 1) || (enemyXTile == playerXTile - 1);
		}

	}

	return isNearby || hasLineOfSight;
}


bool EnemyPatrol::hasReachedPositionTarget() const
{
	VectorF position = mEnemy->position();
	return distanceSquared(position, mEnemy->positionTargetRect()->Center()) < 10.0f;
}
