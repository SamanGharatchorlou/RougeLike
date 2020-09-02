#include "pch.h"
#include "EnemyRun.h"

#include "Actors/Enemies/Enemy.h"
#include "AI/Pathing/AIPathMap.h"


void EnemyRun::init()
{
	mEnemy->animator().selectAnimation(Action::Run);
	updatePath(-1);

	printf("init run\n");
}


void EnemyRun::fastUpdate(float dt)
{
	if(mPath.size() > 0)
		mEnemy->accellerateTowards(mEnemy->pathing()->position(mPath.top()));
}


void EnemyRun::slowUpdate(float dt)
{
	if (!canAttack())
	{
		if (mPath.size() > 0)
		{
			if (!inChaseRange())
			{
				mEnemy->replaceState(EnemyState::Patrol);
			}
			else // Keep chasing!
			{
				VectorF targetPosition = mEnemy->pathing()->position(mPath.top());
				if (distanceSquared(mEnemy->position(), targetPosition) < 5.0f)
				{
					mPath.pop();
				}
			}
		}
		else // Not in attack range and no path left
		{
			updatePath(-1);
		}
	}
	else // Target has been reached, attack!
	{
		printf("start pre attack\n");
		mEnemy->physics()->facePoint(mEnemy->target()->position());
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
		updatePath(-1);
}


void EnemyRun::exit()
{
#if DRAW_AI_PATH
	mEnemy->mDebugger.setPath(Path());
#endif
}


// Generate a new path
void EnemyRun::updatePath(int pathLimit)
{
	mPath = mEnemy->pathing()->findPath(mEnemy->position(), mEnemy->target()->rect().Center(), pathLimit);
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


bool EnemyRun::canAttack() const
{
	return mEnemy->canAttck() && inAttackRange();
}

bool EnemyRun::inAttackRange() const
{
	VectorF position = mEnemy->rect().Center();
	VectorF nearestTargetSide = closestRectSide(position, mEnemy->target()->scaledRect());

	float dis = mEnemy->getAttributeValue(AttributeType::TackleDistance);

	return distance(position, nearestTargetSide) < (mEnemy->getAttributeValue(AttributeType::TackleDistance) * 0.8f);
}


// --- Private Functions ---
bool EnemyRun::inChaseRange() const
{
	return distance(mEnemy->target()->position(), mEnemy->position()) < mEnemy->getAttributeValue(AttributeType::ChaseRange);
}

