#include "pch.h"
#include "EnemyWait.h"
#include "../Enemy.h"


void EnemyWait::init()
{
	timer.restart();	
	mEnemy->animator().selectAnimation("Idle");
}


void EnemyWait::slowUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();

	if (timer.getSeconds() > 2.0f)
		mEnemy->popState();
}


void EnemyWait::render()
{
	mEnemy->renderCharacter();
}