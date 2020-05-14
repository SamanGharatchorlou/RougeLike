#include "pch.h"
#include "EnemyAlert.h"

#include "Objects/Actors/Enemies/Enemy.h"
#include "EnemyRun.h"


void EnemyAlert::init()
{
	mEnemy->animator().selectAnimation("Hit");
	timer.restart();
}


void EnemyAlert::slowUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();

	if (timer.getSeconds() > mEnemy->getPropertyValue("AttentionTime"))
	{
		mEnemy->replaceState(EnemyState::Run);
	}
}


void EnemyAlert::render()
{
	mEnemy->renderCharacter();
}

