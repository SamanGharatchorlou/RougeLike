#include "pch.h"
#include "EnemyRun.h"

#include "Actors/Enemies/Enemy.h"
#include "AI/Pathing/AIPathMap.h"


void EnemyRun::init()
{
	mEnemy->animator().selectAnimation(Animation::Run);
	updatePath(-1);
}


void EnemyRun::fastUpdate(float dt)
{
	if(mPath.size() > 0)
		mEnemy->accellerateTowards(mEnemy->pathing()->position(mPath.top()));
}


void EnemyRun::slowUpdate(float dt)
{
	if (mEnemy->hasTarget())
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

					mEnemy->physics()->facePoint(mEnemy->target()->position());
				}
			}
			else // Not in attack range and no path left
			{
				// Or no valid path was found, try again 
				//mEnemy->addState(EnemyState::Wait);
				updatePath(-1);
			}
		}
		else // Target has been reached, attack!
		{
			mEnemy->physics()->facePoint(mEnemy->target()->position());
			mEnemy->addState(EnemyState::PreAttack);
		}
	}
}


void EnemyRun::render()
{
	mEnemy->renderCharacter();
}


void EnemyRun::resume()
{
	mEnemy->animator().selectAnimation(Animation::Run);
	mPath.empty();
}


void EnemyRun::exit()
{
#if DRAW_AI_PATH
	mEnemy->mDebugger.setPath(Path());
#endif
}


VectorF EnemyRun::targetPosition() const
{
	const AIPathing* pathing = mEnemy->pathing();

	RectF targetRect = mEnemy->target()->rect();
	VectorF points[5] { targetRect.Center(), targetRect.BotLeft(), targetRect.BotRight(), targetRect.TopRight(), targetRect.TopLeft() };
	
	int i = 0;
	bool isValidFloorTile = false;

	while (!isValidFloorTile && i < 5)
	{
		Index targetIndex = pathing->index(points[i++]);
		isValidFloorTile = targetIndex.isPositive() && pathing->map()->floorCollisionTile(targetIndex);
	}

	return isValidFloorTile ? points[i - 1] : points[0];
}


// Generate a new path
void EnemyRun::updatePath(int pathLimit)
{
	VectorF endPosition = targetPosition();
	mPath = mEnemy->pathing()->findPath(mEnemy->position(), endPosition, pathLimit);

#if DRAW_AI_PATH
	mEnemy->mDebugger.setPath(mPath);
#endif
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

	return distance(position, nearestTargetSide) < (mEnemy->getAttributeValue(AttributeType::TackleDistance) * 0.8f);
}


// --- Private Functions ---
bool EnemyRun::inChaseRange() const
{
	return distance(mEnemy->target()->position(), mEnemy->position()) < mEnemy->getAttributeValue(AttributeType::ChaseRange);
}

