#include "pch.h"
#include "EnemyStun.h"
#include "../Enemy.h"

#include "Game/Camera/Camera.h"
#include "Animations/AnimationReader.h"


void EnemyStun::init()
{
	timer.restart();
	mEnemy->animator().selectAnimation(Action::Idle);

	XMLParser parser(FileManager::Get()->findFile(FileManager::Config_Abilities, "Stun"));
	AnimationReader reader;
	mStunAnimation = reader.buildAnimator(parser.rootChild("Animator"));

	mStunAnimation.startAnimation(Action::Active);

	VectorF size = realiseSize(mStunAnimation.frameSize(), mEnemy->rect().Width());
	mStunRect.SetSize(size);
}


void EnemyStun::slowUpdate(float dt)
{
	bool addHitState = false;
	mEnemy->resolveCollisions(addHitState);

	if (timer.getSeconds() > mStunTime)
		mEnemy->popState();

	VectorF position = Camera::Get()->toCameraCoords(mEnemy->rect().TopCenter());
	mStunRect.SetBotCenter(position);

	mStunAnimation.slowUpdate(dt);
}


void EnemyStun::render()
{
	mEnemy->renderCharacter();
	mStunAnimation.render(mStunRect);
}


void EnemyStun::resume()
{
	timer.resume();
}


void EnemyStun::pause()
{
	timer.pause();
}