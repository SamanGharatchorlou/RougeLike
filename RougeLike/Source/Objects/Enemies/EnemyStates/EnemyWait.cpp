#include "pch.h"
#include "EnemyWait.h"
#include "Objects/Enemies/Enemy.h"


void EnemyWait::init()
{
	timer.restart();	
	mEnemy->animator().selectAnimation("Idle");
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