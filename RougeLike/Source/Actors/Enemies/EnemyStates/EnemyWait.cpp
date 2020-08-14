#include "pch.h"
#include "EnemyWait.h"
#include "../Enemy.h"


void EnemyWait::init()
{
	timer.restart();	
	mEnemy->animator().selectAnimation(Action::Idle);

	printf("wait init\n");
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

void EnemyWait::resume()
{

	printf("wait resume\n");
	timer.resume();
}

void EnemyWait::pause()
{
	timer.pause();
}