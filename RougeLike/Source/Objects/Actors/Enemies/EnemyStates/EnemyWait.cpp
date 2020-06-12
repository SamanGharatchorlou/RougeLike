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
	//mEnemy->resolveCollisions();
	// todo need to process damage?

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