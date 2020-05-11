#include "pch.h"
#include "EnemyRun.h"

#include "Objects/Enemies/Enemy.h"
#include "Map/Environment.h"
#include "Map/Map.h"


EnemyRun::EnemyRun(Enemy* enemy) : 
	EnemyState(enemy), 
	mAIPathing(enemy->getMap()) { }


void EnemyRun::init()
{
	mEnemy->animator().selectAnimation("Run");
	updatePath();
}


void EnemyRun::fastUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();

	if(mPath.size() > 0)
		mEnemy->accellerateTowards(mAIPathing.position(mPath.top()));
}


void EnemyRun::slowUpdate(float dt)
{
	if (!inAttackRange())
	{
		if (mPath.size() > 0)
		{
			VectorF targetPosition = mAIPathing.position(mPath.top());

			if (distanceSquared(mEnemy->position(), targetPosition) < 5.0f)
				mPath.pop();

			if (!inChaseRange())
				mEnemy->replaceState(EnemyState::Patrol);
		}
		else // Not in attack range and no path left
		{
			mEnemy->addState(EnemyState::Wait);
		}
	}
	else // Target has been reached, attack!
	{
		mEnemy->addState(EnemyState::PreAttack);
	}

	// Face player
	mEnemy->facePoint(mEnemy->attackTargetRect()->Center());
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
	mEnemy->animator().selectAnimation("Run");
	
	if(!inAttackRange())
		updatePath();
}


// Generate a path to 
void EnemyRun::updatePath()
{
	mPath = mAIPathing.findPath(mEnemy->position(), mEnemy->attackTargetRect()->Center());

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

	return distanceSquared(position, nearestTargetSide) < (mEnemy->getPropertyValue("TackleDistance") * 0.8f);
}


// --- Private Functions ---
bool EnemyRun::inChaseRange() const
{
	return distanceSquared(mEnemy->attackTargetRect()->Center(), mEnemy->position()) < mEnemy->getPropertyValue("ChaseRange");
}