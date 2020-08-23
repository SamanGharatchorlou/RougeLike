#include "pch.h"
#include "EnemyHit.h"

#include "Actors/Enemies/Enemy.h"

#include "Game/Camera/Camera.h"



void EnemyHit::init()
{
	mTimer.start();
	mEnemy->animator().selectAnimation(Action::Hurt);
	mEnemy->animator().activeAimation()->reset();

	// Apply red colour modulation
	mColourMod = RenderColour(225, 0, 0);

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


void EnemyHit::render()
{
	// Flash red
	float flashTime = 0.1f;
	if (mTimer.getSeconds() < flashTime)
	{
		mEnemy->animator().render(mEnemy->renderRect(), mEnemy->physics()->flip(), mColourMod);
	}
	else
	{
		mEnemy->animator().render(mEnemy->renderRect(), mEnemy->physics()->flip());
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
