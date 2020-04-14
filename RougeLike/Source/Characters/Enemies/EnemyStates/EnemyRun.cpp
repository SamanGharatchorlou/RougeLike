#include "pch.h"
#include "EnemyRun.h"

#include "Game/GameData.h"
#include "Map/MapLevel.h"
#include "Map/Map.h"
#include "Characters/Enemies/Enemy.h"


EnemyRun::EnemyRun(Enemy* enemy) : EnemyState(enemy), mAIPathing(enemy->getData()->level->primaryMap()) { }


void EnemyRun::init()
{
	mEnemy->getAnimator()->selectAnimation("Run");

	// Generate AI path to end target
	VectorF currentPosition = mEnemy->getMovement().getPostion();
	VectorF endTargetPosition = mEnemy->targetRect().Center();
	mPath = mAIPathing.findPath(currentPosition, endTargetPosition);
}


void EnemyRun::fastUpdate(float dt)
{

	mEnemy->move(dt);
}


void EnemyRun::slowUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();

	updatePath();

	VectorF targetPosition;

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
			mEnemy->addState(EnemyState::PreAttack);
		}
	}

	updateMovement(targetPosition);
}


void EnemyRun::render()
{
	mEnemy->renderCharacter();
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
	
	float targetDistanceSquared = distance(currentPosition, mEnemy->targetRect().Center());
	float closeDistanceSquared = mEnemy->getData()->level->primaryMap()->getTileSize().x * mEnemy->getData()->level->primaryMap()->getTileSize().x * 1.5;

	// Take a direct route to the end target if close enough (should be >1 tile width, see AIPAthing::nearestFloorTile fix)
	if (targetDistanceSquared < closeDistanceSquared)
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
	VectorF currentPosition = mEnemy->rect().Center();
	VectorF nearestTargetSide = closestRectSide(currentPosition, mEnemy->targetRect());

	return distanceSquared(currentPosition, nearestTargetSide) < (mEnemy->propertyBag().pTackleDistance.get() * 0.8f);
}

bool EnemyRun::inChaseRange() const
{
	return distanceSquared(mEnemy->targetRect().Center(), mEnemy->getMovement().getPostion()) < (mEnemy->propertyBag().pChaseRange.get());
}