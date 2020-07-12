#include "pch.h"
#include "StunEffect.h"

#include "Game/Camera.h"
#include "Objects/Actors/Enemies/Enemy.h"



void StunEffect::clearData()
{
	clearBaseData();

	mMaxSize = 0.0f;
	mTime = 0.0f;
	mAnimator.reset();
}


void StunEffect::init()
{
	mAnimator.startAnimation(Action::Active);

	mMaxSize = mReceiver->rect().Width();

	Enemy* enemy = static_cast<Enemy*>(mReceiver);
	if (enemy)
		enemy->addWaitState(mAnimator.frameCount() * mAnimator.frameTime() * 1.2f);
}


void StunEffect::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	if (mAnimator.loops() > 0)
		endEffect();
}


void StunEffect::render()
{
	VectorF size = realiseSize(mAnimator.frameSize(), mMaxSize);
	RectF rect(VectorF(), size);

	VectorF position = Camera::Get()->toCameraCoords(mReceiver->rect().TopCenter());
	rect.SetBotCenter(position);

	mAnimator.render(rect);
}

