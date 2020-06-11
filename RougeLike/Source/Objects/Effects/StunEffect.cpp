#include "pch.h"
#include "StunEffect.h"

#include "Objects/Actors/Actor.h"
#include "Objects/Actors/Enemies/Enemy.h"

#include "Game/Camera.h"
#include "Objects/Attributes/Health.h"

void StunEffect::init()
{
	mAnimator->startAnimation(Action::Active);

	Enemy* enemy = static_cast<Enemy*>(mActor);
	if (enemy)
	{
		float hp = enemy->getPropertyValue("Health");

		if (hp == 0)
			enemy->addState(EnemyState::Dead);
		else
			enemy->addWaitState(mAnimator->frameCount() * mAnimator->frameTime() * 1.2f);
	}
}


void StunEffect::slowUpdate(float dt)
{
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

