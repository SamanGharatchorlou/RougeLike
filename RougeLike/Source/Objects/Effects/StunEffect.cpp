#include "pch.h"
#include "StunEffect.h"

#include "Objects/Actors/Actor.h"
#include "Objects/Actors/Enemies/Enemy.h"

#include "Game/Camera.h"
#include "Objects/Attributes/Health.h"



void StunEffect::fillData(const EffectPropertyBag* properties)
{
	if (properties->contains("StunTime"))
	{
		Property* property = properties->get("StunTime");
		mTime = property->value();
	}
}


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

