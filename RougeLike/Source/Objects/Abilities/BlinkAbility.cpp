#include "pch.h"
#include "BilnkAbility.h"

#include "Objects/Actors/Actor.h"

#include "Graphics/Texture.h"
#include "Animations/Animator.h"
#include "Game/Camera.h"

#include "Objects/Effects/BlinkEffect.h"

#include "Map/Map.h"

void BlinkAbility::init(Animator animator)
{
	mAnimator = animator;
}


void BlinkAbility::activate(VectorF target)
{
	resetSize();
	mRect.SetCenter(target);
	mTarget = target;
	mAnimator.selectAnimation("activate");
}

void BlinkAbility::activate(Actor* actor)
{
	BlinkEffect* blink = new BlinkEffect(mTarget);
	actor->addEffect(blink);

	mTimer.start();
}


void BlinkAbility::slowUpdate(float dt)
{
	if (mTimer.getSeconds() > 1.0f)
		mRect.SetSize(mRect.Size() * 0.99);

	if(mRect.Size().x < 10.0f)
		setState(Finished);


	mAnimator.slowUpdate(dt);
}



void BlinkAbility::render()
{
	// Center position
	VectorF targetPosition = Camera::Get()->toCameraCoords(mTarget);
	mRect.SetCenter(targetPosition);


	mAnimator.getSpriteTile()->render(mRect);
}


void BlinkAbility::resetSize()
{
	VectorF baseDimentions = mAnimator.getSpriteTile()->getRect().Size();
	VectorF size = baseDimentions * 1.2f;

	mRect.SetSize(size);
}

