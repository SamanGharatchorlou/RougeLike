#include "pch.h"
#include "EnemyAlert.h"

#include "Objects/Enemies/Enemy.h"
#include "EnemyRun.h"


void EnemyAlert::init()
{
	mEnemy->animator().selectAnimation("Hit");
	timer.restart();
}


void EnemyAlert::fastUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();
}


void EnemyAlert::slowUpdate(float dt)
{
	if (timer.getSeconds() > mEnemy->getPropertyValue("AttentionTime"))
	{
		mEnemy->replaceState(EnemyState::Run);
	}
}


void EnemyAlert::render()
{
	mEnemy->renderCharacter();
}

