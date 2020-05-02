#include "pch.h"
#include "EnemyPatrol.h"

#include "Game/GameData.h"

#include "EnemyIdle.h"

#include "Objects/Enemies/Enemy.h"

#include "Map/Environment.h"
#include "Map/Map.h"


EnemyPatrol::EnemyPatrol(Enemy* enemy) : EnemyState(enemy) { }


void EnemyPatrol::init()
{
	mEnemy->getAnimator()->selectAnimation("Run");
	setPatrolPoint();
}


void EnemyPatrol::fastUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();
	mEnemy->accellerateTowards(mEnemy->positionTargetRect()->Center());
}


void EnemyPatrol::slowUpdate(float dt)
{
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
	mEnemy->getAnimator()->selectAnimation("Run");
	setPatrolPoint();
}


// --- Private Functions -- //

void EnemyPatrol::setPatrolPoint()
{
	// TODO: make this enemy get map
	Map* map = mEnemy->getData()->environment->primaryMap();

	VectorF position = mEnemy->position();
	Index tilePositionIndex = map->index(position);

	Index yTileRange = map->findYFloorTileRange(tilePositionIndex.x);

	VectorF highestPoint = map->tileRect(Index(tilePositionIndex.x, yTileRange.x)).Center();
	VectorF lowestPoint = map->tileRect(Index(tilePositionIndex.x, yTileRange.y)).Center();

	// set the furthest point
	VectorF patrolTarget = (position.y - highestPoint.y < lowestPoint.y - position.y) ? lowestPoint : highestPoint;
	ASSERT(Warning, map->isValidPosition(patrolTarget), "Invalid enemy patrol target: %f, %f", patrolTarget.x, patrolTarget.y);

	const MapTile* tile = map->tile(patrolTarget);
	mEnemy->setPositionTarget(tile->rectPtr());
}



bool EnemyPatrol::canSeeAttackTarget() const
{
	Map* map = mEnemy->getData()->environment->primaryMap();

	VectorF position = mEnemy->position();
	VectorF attackTargetPosition = mEnemy->attackTargetRect()->Center();

	bool isNearby = distanceSquared(attackTargetPosition, position) < mEnemy->propertyBag().pSightRange.get();
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
			int enemyXTile = map->index(position).x;
			int playerXTile = map->index(attackTargetPosition).x;
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