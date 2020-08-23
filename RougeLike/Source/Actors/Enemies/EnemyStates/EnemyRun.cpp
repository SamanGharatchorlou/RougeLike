#include "pch.h"
#include "EnemyRun.h"

#include "Actors/Enemies/Enemy.h"

#include "AI/AIPathMap.h"

void EnemyRun::init()
{
	mEnemy->animator().selectAnimation(Action::Run);
	updatePath();
}


void EnemyRun::fastUpdate(float dt)
{
	if(mPath.size() > 0)
		mEnemy->accellerateTowards(mEnemy->getPathMap().position(mPath.top()));


}


void EnemyRun::slowUpdate(float dt)
{
	mEnemy->resolveCollisions();

	if (!inAttackRange())
	{
		if (mPath.size() > 0)
		{
			if (!inChaseRange())
			{
				mEnemy->replaceState(EnemyState::Patrol);
			}
			else // Keep chasing!
			{
				VectorF targetPosition = mEnemy->getPathMap().position(mPath.top());
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
	mEnemy->physics()->facePoint(mEnemy->target()->position());
}


void EnemyRun::render()
{
	mEnemy->renderCharacter();
}


void EnemyRun::resume()
{
	mEnemy->animator().selectAnimation(Action::Run);
	
	if(!inAttackRange())
		updatePath();
}


void EnemyRun::exit()
{
#if DRAW_AI_PATH
	mEnemy->mDebugger.setPath(Path());
#endif
}


// Generate a new path
void EnemyRun::updatePath()
{
	mPath = mEnemy->getPathMap().findPath(mEnemy->position(), mEnemy->target()->rect().BotCenter());
#if DRAW_AI_PATH
	mEnemy->mDebugger.setPath(mPath);
#endif

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
	VectorF nearestTargetSide = closestRectSide(position, mEnemy->target()->scaledRect());

	return distance(position, nearestTargetSide) < (mEnemy->getAttributeValue(AttributeType::TackleDistance) * 0.8f);
}


// --- Private Functions ---
bool EnemyRun::inChaseRange() const
{
	return distance(mEnemy->target()->position(), mEnemy->position()) < mEnemy->getAttributeValue(AttributeType::ChaseRange);
}

