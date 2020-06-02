#include "pch.h"
#include "EnemyRun.h"

#include "Objects/Actors/Enemies/Enemy.h"
#include "Map/Map.h"


EnemyRun::EnemyRun(Enemy* enemy) : 
	EnemyState(enemy), 
	mAIPathing(enemy->getPathMap()) { }


void EnemyRun::init()
{
	mEnemy->animator().selectAnimation("Run");
	updatePath();
}


void EnemyRun::fastUpdate(float dt)
{
	if(mPath.size() > 0)
		mEnemy->accellerateTowards(mAIPathing.position(mPath.top()));
}


void EnemyRun::slowUpdate(float dt)
{
	mEnemy->resolveCollisions();

	if (!inAttackRange())
	{
		if (mPath.size() > 0)
		{
			if (!inChaseRange()) // Out of chase range
			{
				mEnemy->replaceState(EnemyState::Patrol);
			}
			else // Keep chasing!
			{
				VectorF targetPosition = mAIPathing.position(mPath.top());
				if (distanceSquared(mEnemy->position(), targetPosition) < 5.0f)
				{
					mPath.pop();

					// Random chance to update own path
					if (randomNumberBetween(0, 100) >= 75)
						updatePath();
				}
			}
		}
		else // Not in attack range and no path left
		{
			updatePath();
		}
	}
	else // Target has been reached, attack!
	{
		mEnemy->addState(EnemyState::PreAttack);
	}

	// Face player
	mEnemy->physics()->facePoint(mEnemy->attackTargetRect()->Center());
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


// Generate a new path
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