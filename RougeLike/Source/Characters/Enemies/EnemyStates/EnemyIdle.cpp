#include "pch.h"
#include "EnemyIdle.h"

#include "Characters/Enemies/Enemy.h"


void EnemyIdle::init()
{
	timer.restart();
	mEnemy->getAnimator()->selectAnimation("Idle");
}


void EnemyIdle::slowUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();

	float idleTime = mEnemy->propertyBag().pIdleTime.get();;

	if (idleTime > 0 && timer.getSeconds() > idleTime)
	{
		mEnemy->replaceState(EnemyState::Patrol);
	}

	if (canSeeTarget())
	{
		mEnemy->replaceState(EnemyState::Alert);
	}
}



bool EnemyIdle::canSeeTarget() const
{
	//float distance = distanceSquared(mEnemy->targetRect().Center(), mEnemy->getMovement().getPostion());
	return false;// distance < mEnemy->propertyBag().pSightRange.get();
}


void EnemyIdle::render()
{
	mEnemy->renderCharacter();
}