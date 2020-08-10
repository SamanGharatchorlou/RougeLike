#include "pch.h"
#include "EnemyAlert.h"

#include "Actors/Enemies/Enemy.h"
#include "EnemyRun.h"


void EnemyAlert::init()
{
	mEnemy->animator().selectAnimation(Action::Alert);
	mEnemy->physics()->facePoint(mEnemy->target()->position());
}


void EnemyAlert::slowUpdate(float dt)
{
	mEnemy->resolveCollisions();

	if (mEnemy->animator().loops() > 0)
	{
		mEnemy->replaceState(EnemyState::Run);
		printf("run");
	}
}


void EnemyAlert::render()
{
	mEnemy->renderCharacter();
}

