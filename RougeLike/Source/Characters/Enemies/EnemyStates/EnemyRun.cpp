#include "pch.h"
#include "EnemyRun.h"

#include "Game/GameData.h"
#include "Map/MapLevel.h"
#include "Map/Map.h"
#include "Characters/Enemies/Enemy.h"


EnemyRun::EnemyRun(Enemy* enemy) : 
	EnemyState(enemy), 
	mAIPathing(enemy->getMap()) { }


void EnemyRun::init()
{
	mEnemy->getAnimator()->selectAnimation("Run");
	updatePath();
}


void EnemyRun::fastUpdate(float dt)
{
	if(mPath.size() > 0)
		mEnemy->moveTowards(mAIPathing.position(mPath.top()));
}


void EnemyRun::slowUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();

	VectorF targetPosition;

	if (!inAttackRange())
	{
		if (mPath.size() > 0)
		{
			targetPosition = mAIPathing.position(mPath.top());

			if (distanceSquared(position(), targetPosition) < 5.0f)
				mPath.pop();

			if (!inChaseRange())
				mEnemy->replaceState(EnemyState::Patrol);
		}
		// Not in attack range and no path left
		else
		{
			mEnemy->addState(EnemyState::Wait);
		}
	}
	// Target has been reached, attack!
	else
	{
		mEnemy->addState(EnemyState::PreAttack);
	}

	//mEnemy->getMovement().setDirection(targetPosition - position());
}


void EnemyRun::render()
{
#if DRAW_AI_PATH
	mAIPathing.draw();
#endif
	mEnemy->renderCharacter();
}


void EnemyRun::resume()
{
	mEnemy->getAnimator()->selectAnimation("Run");
	
	if(!inAttackRange())
		updatePath();
}


// Generate a path to 
void EnemyRun::updatePath()
{
	mPath = mAIPathing.findPath(position(), mEnemy->attackTargetRect()->Center());

	// No valid path was found, wait a bit then try again 
	if (mPath.size() == 0)
		mEnemy->addState(EnemyState::Wait);
}


Index EnemyRun::nextTileIndex()
{
	return mPath.size() > 0 ? mPath.top() : Index(-1, -1);
}


bool EnemyRun::inAttackRange() const
{
	VectorF position = mEnemy->rect().Center();
	VectorF nearestTargetSide = closestRectSide(position, *mEnemy->attackTargetRect());

	return distanceSquared(position, nearestTargetSide) < (mEnemy->propertyBag().pTackleDistance.get() * 0.8f);
}


// --- Private Functions ---

VectorF EnemyRun::position() const
{
	return mEnemy->physics().position();
}



bool EnemyRun::inChaseRange() const
{
	return distanceSquared(mEnemy->attackTargetRect()->Center(), position()) < (mEnemy->propertyBag().pChaseRange.get());
}