#include "pch.h"
#include "EnemyRun.h"

#include "Game/GameData.h"
#include "Characters/Enemies/Enemy.h"


EnemyRun::EnemyRun(Enemy* enemy) : 
	mEnemy(enemy), 
	mAIPathing(enemy->getData()->map) { }


void EnemyRun::init()
{
	mEnemy->getAnimator()->selectAnimation("Run");

	// Generate AI path to end target
	VectorF currentPosition = mEnemy->getMovement().getPostion();
	VectorF endTargetPosition = mEnemy->targetRect().Center();
	mPath = mAIPathing.findPath(currentPosition, endTargetPosition);
}


void EnemyRun::slowUpdate(float dt)
{
	VectorF targetPosition;

	updatePath();

	// Target has not been reached yet, move to next tile
	if (mPath.size() > 0)
	{
		targetPosition = mAIPathing.getTilePosition(mPath.top());

		if(!inChaseRange())
		{
			mEnemy->replaceState(EnemyState::Patrol);
		}
	}
	// Target has been reached, move directly to target
	else
	{
		targetPosition = mEnemy->targetRect().Center();

		if (inAttackRange())
		{
			mEnemy->addState(EnemyState::Attack);
		}
	}

	updateMovement(targetPosition);
}


void EnemyRun::fastUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();

	mEnemy->move(dt);
}


void EnemyRun::resume()
{
	mEnemy->getAnimator()->selectAnimation("Run");
}


// --- Private Functions ---

void EnemyRun::updateMovement(VectorF target)
{
	VectorF currentPosition = mEnemy->getMovement().getPostion();
	VectorF direction = target - currentPosition;

	mEnemy->getMovement().setDirection(direction);
}


// Generate a path to 
void EnemyRun::updatePath()
{
	VectorF currentPosition = mEnemy->getMovement().getPostion();
	
	// Take a direct route to the end target if close enough
	if (distanceSquared(currentPosition, mEnemy->targetRect().Center()) < 2500.0f)
	{
		mPath = std::stack<Vector2D<int>>();
	}
	// Expensive operation - AI path finder: update the path every 20 ticks or so
	else if (randomNumberBetween(0, 20) > 18)
	{
		mPath = mAIPathing.findPath(currentPosition, mEnemy->targetRect().Center());
	}
}

bool EnemyRun::inAttackRange() const
{
	VectorF currentPosition = mEnemy->getRect().Center();
	VectorF nearestTargetSide = closestRectSide(currentPosition, mEnemy->targetRect());

	return distanceSquared(currentPosition, nearestTargetSide) < (mEnemy->propertyBag().pTackleDistance.get() * 0.8f);
}

bool EnemyRun::inChaseRange() const
{
	return distanceSquared(mEnemy->targetRect().Center(), mEnemy->getMovement().getPostion()) < (mEnemy->propertyBag().pChaseRange.get());
}