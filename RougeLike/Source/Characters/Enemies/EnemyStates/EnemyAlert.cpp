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
	// Collisions
	mEnemy->resolvePlayerWeaponCollisions();

	// Flip sprite half way
	if (timer.getSeconds() > mEnemy->propertyBag().pAttentionTime.get() / 2.0f)
	{
		mEnemy->getMovement().flipDirection();
	}

	if (timer.getSeconds() > mEnemy->propertyBag().pAttentionTime.get())
	{
		mEnemy->replaceState(EnemyState::Run);
		mEnemy->getMovement().flipDirection();
	}
}


void EnemyAlert::render()
{
	mEnemy->renderCharacter();
}

