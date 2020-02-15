#include "pch.h"
#include "EnemyDead.h"

#include "Characters/Enemies/Enemy.h"
#include "Graphics/Texture.h"


void EnemyDead::init()
{
	mEnemy->getAnimator()->selectAnimation("Dead");


	EnemyDeadData event;
	event.score = 50;

	//mEnemy->notify(Event::EnemyDead, event);
}

void EnemyDead::slowUpdate(float dt)
{
	Texture* texture = mEnemy->getAnimator()->getSpriteTexture();

	if (mEnemy->getAnimator()->loopCount() > 0)
	{
		mEnemy->getAnimator()->stop();
		texture->modifyAlpha(-100 * dt);
	}

	// Remove enemy from play
	if (texture->alpha() <= 10)
	{
		mEnemy->setActive(false);
	}
}