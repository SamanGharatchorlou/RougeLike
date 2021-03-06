#include "pch.h"
#include "EnemyWait.h"
#include "../Enemy.h"


void EnemyWait::init()
{
	timer.restart();	
	mEnemy->animator().selectAnimation(Animation::Idle);
}


void EnemyWait::slowUpdate(float dt)
{
	if (timer.getSeconds() > mWaitTime)
		mEnemy->popState();
}


void EnemyWait::render()
{
	mEnemy->renderCharacter();
}

void EnemyWait::resume()
{
	timer.resume();
}

void EnemyWait::pause()
{
	timer.pause();
}