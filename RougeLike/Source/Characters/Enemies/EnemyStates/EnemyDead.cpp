#include "pch.h"
#include "EnemyDead.h"

#include "Characters/Enemies/Enemy.h"


void EnemyDead::init()
{
	mEnemy->getAnimator()->selectAnimation("Dead");

	// Let everyone know he has died
	int score = mEnemy->propertyBag().pScore.get();
	int exp = mEnemy->propertyBag().pExp.get();
	EnemyDeadEvent* dataPtr = new EnemyDeadEvent(score, exp);

	mEnemy->pushEvent(EventPacket(Event::EnemyDead, dataPtr));
}

void EnemyDead::slowUpdate(float dt)
{
	Texture* texture = mEnemy->getAnimator()->getSpriteTexture();

	if (mEnemy->getAnimator()->loopCount() > 0)
	{
		mEnemy->getAnimator()->stop();
		mEnemy->alpha() -= (Uint8)(100 * dt);
	}

	// Remove enemy from play
	if (mEnemy->alpha() <= 10)
	{
		mEnemy->setActive(false);
	}
}