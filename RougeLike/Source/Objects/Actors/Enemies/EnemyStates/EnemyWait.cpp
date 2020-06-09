#include "pch.h"
#include "EnemyWait.h"
#include "../Enemy.h"


void EnemyWait::init()
{
	timer.restart();	
	mEnemy->animator().selectAnimation(Action::Idle);
}


void EnemyWait::slowUpdate(float dt)
{
	mEnemy->resolveCollisions();

	if (timer.getSeconds() > mWaitTime)
		mEnemy->popState();
}


void EnemyWait::render()
{
	mEnemy->renderCharacter();
}