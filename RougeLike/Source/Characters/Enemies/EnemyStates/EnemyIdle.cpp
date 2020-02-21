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
	if (idleTime > 0 && timer.getSeconds() > idleTime)
	{
		mEnemy->replaceState(EnemyState::Patrol);
	}

	if (canSeeTarget())
	{
		mEnemy->replaceState(EnemyState::Alert);
	}
}


void EnemyIdle::fastUpdate(float dt)
{
	// Collisions
	mEnemy->resolvePlayerWeaponCollisions();
}

bool EnemyIdle::canSeeTarget() const
{
	float distance = distanceSquared(mEnemy->targetRect().Center(), mEnemy->getMovement().getPostion());
	return distance < mEnemy->propertyBag().pSightRange.get();
}