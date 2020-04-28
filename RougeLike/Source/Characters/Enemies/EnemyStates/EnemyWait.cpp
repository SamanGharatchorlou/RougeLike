#include "pch.h"
#include "EnemyWait.h"
#include "Characters/Enemies/Enemy.h"


void EnemyWait::init()
{
	timer.restart();	
	mEnemy->getAnimator()->selectAnimation("Idle");
}


void EnemyWait::slowUpdate(float dt)
{
	if (timer.getSeconds() > 2.0f)
		mEnemy->popState();
}


void EnemyWait::render()
{
	mEnemy->renderCharacter();
}