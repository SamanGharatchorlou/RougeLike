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
	mEnemy->move(dt);
}


void EnemyRun::slowUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();

	VectorF targetPosition;

	// Target has not been reached yet, move to next tile
	if (!inAttackRange())
	{
		targetPosition = mAIPathing.getTilePosition(mPath.top());

		if (distanceSquared(position(), targetPosition) < 5.0f)
		{
			UpdateAIPathMapEvent* dataPtr = new UpdateAIPathMapEvent;
			EventPacket eventPacket(Event::UpdateAIPathMap, dataPtr);
			mEnemy->pushEvent(eventPacket);
		}

		if(!inChaseRange())
		{
			mEnemy->replaceState(EnemyState::Patrol);
		}
	}
	// Target has been reached, attack!
	else
	{
		mEnemy->addState(EnemyState::PreAttack);
	}

	updateMovement(targetPosition);
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
	updatePath();
}


// Generate a path to 
void EnemyRun::updatePath()
{
	mPath = mAIPathing.findPath(position(), mEnemy->targetRect().Center());
}


Index EnemyRun::nextTileIndex()
{
	return mPath.size() > 0 ? mPath.top() : Index(-1, -1);
}


// --- Private Functions ---

VectorF EnemyRun::position() const
{
	return mEnemy->getMovement().getPostion();
}

void EnemyRun::updateMovement(VectorF target)
{
	VectorF direction = target - position();
	mEnemy->getMovement().setDirection(direction);
}

bool EnemyRun::inAttackRange() const
{
	VectorF position = mEnemy->rect().Center();
	VectorF nearestTargetSide = closestRectSide(position, mEnemy->targetRect());

	return distanceSquared(position, nearestTargetSide) < (mEnemy->propertyBag().pTackleDistance.get() * 0.8f);
}

bool EnemyRun::inChaseRange() const
{
	return distanceSquared(mEnemy->targetRect().Center(), position()) < (mEnemy->propertyBag().pChaseRange.get());
}