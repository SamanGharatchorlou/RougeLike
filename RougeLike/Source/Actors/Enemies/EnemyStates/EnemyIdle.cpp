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
	bool canSeeTarget = false;

	if (mEnemy->hasTarget())
	{
		float targetDistance = distance(mEnemy->target()->position(), mEnemy->position());
		canSeeTarget = targetDistance < mEnemy->getAttributeValue(AttributeType::SightRange);
	}

	return canSeeTarget;
}


void EnemyIdle::render()
{
	mEnemy->renderCharacter();
}

void EnemyIdle::resume()
{
	mEnemy->animator().selectAnimation(Action::Idle);
}