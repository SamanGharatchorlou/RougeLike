#include "pch.h"
#include "EnemyPatrol.h"
#include "EnemyIdle.h"

#include "Characters/Enemies/Enemy.h"


#include "Map/Map.h"

// test
#include "Input/InputManager.h"
#include "Game/GameData.h"


EnemyPatrol::EnemyPatrol(Enemy* enemy) : mEnemy(enemy)
{ }


void EnemyPatrol::init()
{
	mEnemy->getAnimator()->selectAnimation("Run");
	setPatrolPoint();

	VectorF position = mEnemy->getMovement().getPostion();
	mEnemy->getMovement().setDirection(mPatrolTarget - position);
}


void EnemyPatrol::slowUpdate(float dt)
{
	if (hasReachedPatrolTarget())
	{
		EnemyIdle* idleState = new EnemyIdle(mEnemy);
		idleState->setIdleTime(2.0f);

		mEnemy->getStateMachine()->replaceState(idleState);
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
	float distance = distanceSquared(mEnemy->targetRect().Center(), mEnemy->getMovement().getPostion());
	return distance < mEnemy->propertyBag().pSightRange.get();
}


bool EnemyPatrol::hasReachedPatrolTarget() const
{
	VectorF position = mEnemy->getMovement().getPostion();
	return distanceSquared(position, mPatrolTarget) < 10.0f;
}