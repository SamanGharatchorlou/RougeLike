#include "pch.h"
#include "EnemyPatrol.h"
#include "EnemyIdle.h"

#include "Characters/Enemies/Enemy.h"


#include "Map/Map.h"

// test
#include "Input/InputManager.h"
#include "Game/GameData.h"


EnemyPatrol::EnemyPatrol(Enemy* enemy) : EnemyState(enemy) { }


void EnemyPatrol::init()
{
	mEnemy->getAnimator()->selectAnimation("Run");
	setPatrolPoint();

	VectorF position = mEnemy->getMovement().getPostion();
	mEnemy->getMovement().setDirection(mPatrolTarget - position);
}


void EnemyPatrol::slowUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();

	if (hasReachedPatrolTarget())
	{
		mEnemy->getStateMachine()->replaceState(new EnemyIdle(mEnemy));
	}

	if (canSeeTarget())
	{
		mEnemy->replaceState(EnemyState::Alert);
	}
}


void EnemyPatrol::fastUpdate(float dt)
{
	mEnemy->move(dt);
}


void EnemyPatrol::render()
{
	mEnemy->renderCharacter();
}


// --- Private Functions -- //

void EnemyPatrol::setPatrolPoint()
{
	Map* map = mEnemy->getData()->map;

	VectorF position = mEnemy->getMovement().getPostion();
	Vector2D<int> tilePositionIndex = map->getIndex(position);

	Vector2D<int> yTileRange = map->findYFloorTileRange(tilePositionIndex.x);

	VectorF highestPoint = map->getTileRect(tilePositionIndex.x, yTileRange.x).Center();
	VectorF lowestPoint = map->getTileRect(tilePositionIndex.x, yTileRange.y).Center();

	// set the furthest point
	mPatrolTarget = (position.y - highestPoint.y < lowestPoint.y - position.y) ? lowestPoint : highestPoint;
}


bool EnemyPatrol::canSeeTarget() const
{
	Map* map = mEnemy->getData()->map;

	VectorF position = mEnemy->getRect().Center();
	VectorF targetPosition = mEnemy->targetRect().Center();

	float distance = distanceSquared(targetPosition, position);

	bool isNearby = distance < mEnemy->propertyBag().pSightRange.get();
	bool hasLineOfSight = false;

	if (!isNearby)
	{
		// Is there a line of sight?
		float yPatrolDirection = (mPatrolTarget - position).y;
		float yTargetDirection = (targetPosition - position).y;

		bool isFacingTarget = (yPatrolDirection == yTargetDirection);

		if (isFacingTarget)
		{
			int enemyXTile = map->getIndex(position).x;
			int playerXTile = map->getIndex(targetPosition).x;

			hasLineOfSight = (enemyXTile == playerXTile);
		}

	}

	return isNearby || hasLineOfSight;
}


bool EnemyPatrol::hasReachedPatrolTarget() const
{
	VectorF position = mEnemy->getMovement().getPostion();
	return distanceSquared(position, mPatrolTarget) < 10.0f;
}