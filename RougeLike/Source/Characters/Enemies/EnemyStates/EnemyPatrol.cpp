#include "pch.h"
#include "EnemyPatrol.h"
#include "EnemyIdle.h"

#include "Characters/Enemies/Enemy.h"

#include "Map/MapLevel.h" // TODO: do I need both map level and map here?
#include "Map/Map.h"

// test
#include "Input/InputManager.h"
#include "Game/GameData.h"


EnemyPatrol::EnemyPatrol(Enemy* enemy) : EnemyState(enemy) { }


void EnemyPatrol::init()
{
	mEnemy->getAnimator()->selectAnimation("Run");
	setPatrolPoint();
}


void EnemyPatrol::slowUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();


	if (hasReachedPositionTarget())
		mEnemy->getStateMachine()->replaceState(new EnemyIdle(mEnemy));

	if (canSeeAttackTarget())
		mEnemy->replaceState(EnemyState::Alert);
}


void EnemyPatrol::fastUpdate(float dt)
{
	mEnemy->moveTowards(mEnemy->positionTargetRect()->Center());
}


void EnemyPatrol::render()
{
	mEnemy->renderCharacter();
}


// --- Private Functions -- //

void EnemyPatrol::setPatrolPoint()
{
	// TODO: make this enemy get map
	Map* map = mEnemy->getData()->level->primaryMap();

	VectorF position = mEnemy->physics().position();
	Vector2D<int> tilePositionIndex = map->index(position);

	Vector2D<int> yTileRange = map->findYFloorTileRange(tilePositionIndex.x);

	VectorF highestPoint = map->tileRect(tilePositionIndex.x, yTileRange.x).Center();
	VectorF lowestPoint = map->tileRect(tilePositionIndex.x, yTileRange.y).Center();

	// set the furthest point
	VectorF patrolTarget = (position.y - highestPoint.y < lowestPoint.y - position.y) ? lowestPoint : highestPoint;
	ASSERT(Warning, map->isValidPosition(patrolTarget), "Invalid enemy patrol target: %f, %f", patrolTarget.x, patrolTarget.y);

	const MapTile* tile = map->tile(patrolTarget);
	mEnemy->setPositionTarget(tile->rectPtr());
}



bool EnemyPatrol::canSeeAttackTarget() const
{
	Map* map = mEnemy->getData()->level->primaryMap();

	VectorF position = mEnemy->position();
	VectorF attackTargetPosition = mEnemy->attackTargetRect()->Center();

	float distance = distanceSquared(attackTargetPosition, position);

	bool isNearby = distance < mEnemy->propertyBag().pSightRange.get();
	bool hasLineOfSight = false;

	if (!isNearby)
	{
		// Is there a line of sight?
		float yPatrolDirection = (mEnemy->positionTargetRect()->Center() - position).y;
		float yTargetDirection = (attackTargetPosition - position).y;

		bool isFacingTarget = (yPatrolDirection == yTargetDirection);

		if (isFacingTarget)
		{
			int enemyXTile = map->index(position).x;
			int playerXTile = map->index(attackTargetPosition).x;

			hasLineOfSight = (enemyXTile == playerXTile);
		}

	}

	return isNearby || hasLineOfSight;
}


bool EnemyPatrol::hasReachedPositionTarget() const
{
	VectorF position = mEnemy->physics().position();
	return distanceSquared(position, mEnemy->positionTargetRect()->Center()) < 10.0f;
}