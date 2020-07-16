#include "pch.h"
#include "EnemyIdle.h"

#include "Actors/Enemies/Enemy.h"


EnemyIdle::EnemyIdle(Enemy* enemy, float time) : EnemyState(enemy), mTime(time)
{
	mTimer.start();
}


EnemyIdle::EnemyIdle(Enemy* enemy) : EnemyState(enemy), mTime(1.0f) 
{
	mTimer.stop();
}


void EnemyIdle::init()
{
	mEnemy->animator().selectAnimation(Action::Idle);
	mEnemy->physics()->setFlip(static_cast<SDL_RendererFlip>(randomNumberBetween(0, 2)));
}


void EnemyIdle::slowUpdate(float dt)
{
	mEnemy->resolveCollisions();

	if (canSeeAttackTarget())
		mEnemy->replaceState(EnemyState::Alert);

	if (mTimer.getSeconds() >= mTime)
		mEnemy->popState();
}


bool EnemyIdle::canSeeAttackTarget() const
{
	if (mEnemy->hasTarget())
	{
		float targetDistanceSqrd = distanceSquared(mEnemy->target()->position(), mEnemy->position());
		return targetDistanceSqrd < mEnemy->getPropertyValue("SightRange");
	}
	else
		return false;
}


void EnemyIdle::render()
{
	mEnemy->renderCharacter();
}