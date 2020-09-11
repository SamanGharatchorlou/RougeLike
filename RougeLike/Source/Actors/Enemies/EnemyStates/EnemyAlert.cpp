#include "pch.h"
#include "EnemyAlert.h"

#include "Actors/Enemies/Enemy.h"
#include "EnemyRun.h"


void EnemyAlert::init()
{
	mEnemy->animator().selectAnimation(Animation::Alert);
	mEnemy->physics()->facePoint(mEnemy->target()->position());
}


void EnemyAlert::slowUpdate(float dt)
{
	if (mEnemy->animator().loops() > 0)
	{
		mEnemy->replaceState(EnemyState::Run);
	}
}


void EnemyAlert::render()
{
	mEnemy->renderCharacter();
}

