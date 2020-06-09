#include "pch.h"
#include "StunEffect.h"

#include "Objects/Actors/Actor.h"
#include "Objects/Actors/Enemies/Enemy.h"

#include "Game/Camera.h"


void StunEffect::init()
{
	mAnimator->selectAnimation(Action::Active);

	mEnemy = dynamic_cast<Enemy*>(mActor);
	if (mEnemy == nullptr)
	{
		endEffect();
	}
}


void StunEffect::slowUpdate(float dt)
{
	if (mActor->animator().currentAction() != Action::Hurt && mActor->animator().currentAction() != Action::Idle)
	{
		mAnimator->start();

		Enemy* enemy = dynamic_cast<Enemy*>(mActor);
		if (enemy)
		{
			enemy->addWaitState(mAnimator->frameCount() * mAnimator->frameTime());
		}
	}

	mAnimator->slowUpdate(dt);

	if (mAnimator->loops() > 0)
		endEffect();
}


void StunEffect::render()
{
	RectF rect(VectorF(), mSize);
	VectorF position = Camera::Get()->toCameraCoords(mActor->rect().TopCenter());
	rect.SetBotCenter(position);

	mAnimator->render(rect);
}

