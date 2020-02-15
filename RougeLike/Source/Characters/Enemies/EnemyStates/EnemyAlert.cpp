#include "pch.h"
#include "EnemyAlert.h"

#include "Characters/Enemies/Enemy.h"
#include "EnemyRun.h"


void EnemyAlert::init()
{
	mEnemy->getAnimator()->selectAnimation("Hit");

	VectorF position = mEnemy->getMovement().getPostion();
	mEnemy->getMovement().setDirection(position - mEnemy->targetRect().Center());

	mEnemy->getMovement().flipDirection();
	timer.restart();
}

void EnemyAlert::slowUpdate(float dt)
{
	// Flip sprite half way
	if (!mHasFlipped && timer.getSeconds() > mEnemy->attentionTime() / 2.0f)
	{
		mHasFlipped = true;
		mEnemy->getMovement().flipDirection();
	}

	if (timer.getSeconds() > mEnemy->attentionTime())
	{
		mEnemy->replaceState(EnemyState::Run);
		mEnemy->getMovement().flipDirection();
	}
}

void EnemyAlert::fastUpdate(float dt)
{
	// Collisions
	mEnemy->resolvePlayerWeaponCollisions();
}

