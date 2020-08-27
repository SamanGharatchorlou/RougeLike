#include "pch.h"
#include "EnemyHit.h"

#include "Actors/Enemies/Enemy.h"

#include "Game/Camera/Camera.h"



void EnemyHit::init()
{
	mTimer.start();
	mEnemy->animator().selectAnimation(Action::Hurt);
	mEnemy->animator().activeAimation()->reset();

	if(mEnemy->hasTarget())
		mEnemy->physics()->facePoint(mEnemy->target()->position());
}


void EnemyHit::slowUpdate(float dt)
{
	mEnemy->resolveCollisions();

	if (mEnemy->animator().loops() > 0)
	{
		mEnemy->popState();
	}
}


void EnemyHit::resume()
{
	mEnemy->popState();
}


void EnemyHit::exit()
{
	mTimer.stop();
}
