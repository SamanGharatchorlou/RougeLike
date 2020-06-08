#include "pch.h"
#include "EnemyIdle.h"

#include "Objects/Actors/Enemies/Enemy.h"


void EnemyIdle::init()
{
	timer.restart();
	mEnemy->animator().selectAnimation(Action::Idle);

	mEnemy->physics()->setFlip(static_cast<SDL_RendererFlip>(randomNumberBetween(0, 2)));
}


void EnemyIdle::slowUpdate(float dt)
{
	mEnemy->resolveCollisions();

	if (canSeeAttackTarget())
		mEnemy->replaceState(EnemyState::Alert);
}


bool EnemyIdle::canSeeAttackTarget() const
{
	float targetDistanceSqrd = distanceSquared(mEnemy->target()->position(), mEnemy->position());
	return targetDistanceSqrd < mEnemy->getPropertyValue("SightRange");
}


void EnemyIdle::render()
{
	mEnemy->renderCharacter();
}