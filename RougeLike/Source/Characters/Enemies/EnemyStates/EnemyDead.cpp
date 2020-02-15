#include "pch.h"
#include "EnemyDead.h"

#include "Characters/Enemies/Enemy.h"


void EnemyDead::init()
{
	mEnemy->getAnimator()->selectAnimation("Dead");

	EnemyDeadData event;
	event.score = 50;

	mEnemy->notify(Event::EnemyDead, event);
}

void EnemyDead::slowUpdate(float dt)
{
	Texture* texture = mEnemy->getAnimator()->getSpriteTexture();

	if (mEnemy->getAnimator()->loopCount() > 0)
	{
		mEnemy->getAnimator()->stop();
		mEnemy->alpha() -= 100 * dt;
	}

	// Remove enemy from play
	if (mEnemy->alpha() <= 10)
	{
		mEnemy->setActive(false);
	}
}