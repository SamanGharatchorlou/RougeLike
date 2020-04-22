#include "pch.h"
#include "EnemyAlert.h"

#include "Characters/Enemies/Enemy.h"
#include "EnemyRun.h"


void EnemyAlert::init()
{
	mEnemy->getAnimator()->selectAnimation("Hit");

	//VectorF position = mEnemy->getMovement().getPostion();
	//mEnemy->getMovement().setDirection(position - mEnemy->targetPosition());

	//mEnemy->getMovement().flipDirection();
	timer.restart();
}


void EnemyAlert::slowUpdate(float dt)
{
	// Collisions
	mEnemy->resolvePlayerWeaponCollisions();

	//// Flip sprite half way
	//if (flipCounter == 0 && timer.getSeconds() > mEnemy->propertyBag().pAttentionTime.get() / 2.0f)
	//{
	//	mEnemy->getMovement().flipDirection();
	//	flipCounter++;
	//}

	if (timer.getSeconds() > mEnemy->propertyBag().pAttentionTime.get())
	{
		mEnemy->replaceState(EnemyState::Run);
		//mEnemy->getMovement().flipDirection();
		//flipCounter++;
	}
}


void EnemyAlert::render()
{
	mEnemy->renderCharacter();
}

