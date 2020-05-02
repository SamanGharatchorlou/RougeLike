#include "pch.h"
#include "EnemyIdle.h"

#include "Objects/Enemies/Enemy.h"


void EnemyIdle::init()
{
	timer.restart();
	mEnemy->getAnimator()->selectAnimation("Idle");

	mEnemy->setFlip(static_cast<SDL_RendererFlip>(randomNumberBetween(0, 2)));
}

void EnemyIdle::fastUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();
}


void EnemyIdle::slowUpdate(float dt)
{
	if (canSeeAttackTarget())
		mEnemy->replaceState(EnemyState::Alert);
}


bool EnemyIdle::canSeeAttackTarget() const
{
	VectorF attackTargetPosition = mEnemy->attackTargetRect()->Center();
	return distanceSquared(attackTargetPosition, mEnemy->position()) < mEnemy->propertyBag().pSightRange.get();
}


void EnemyIdle::render()
{
	mEnemy->renderCharacter();
}