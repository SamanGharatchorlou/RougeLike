#include "pch.h"
#include "EnemyIdle.h"

#include "Objects/Enemies/Enemy.h"


void EnemyIdle::init()
{
	timer.restart();
	mEnemy->animator().selectAnimation("Idle");

	mEnemy->physics()->setFlip(static_cast<SDL_RendererFlip>(randomNumberBetween(0, 2)));
}


void EnemyIdle::slowUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();

	if (canSeeAttackTarget())
		mEnemy->replaceState(EnemyState::Alert);
}


bool EnemyIdle::canSeeAttackTarget() const
{
	VectorF attackTargetPosition = mEnemy->attackTargetRect()->Center();
	return distanceSquared(attackTargetPosition, mEnemy->position()) < mEnemy->getPropertyValue("SightRange");
}


void EnemyIdle::render()
{
	mEnemy->renderCharacter();
}