#include "pch.h"
#include "BilnkAbility.h"

#include "Objects/Actors/Player/Player.h"
#include "Objects/Actors/Actor.h"

#include "Graphics/Texture.h"
#include "Animations/Animator.h"
#include "Game/Camera.h"

#include "Objects/Effects/BlinkEffect.h"

#include "Map/Map.h"


void BlinkAbility::fillValues(ValueMap& values)
{
	mRange = std::stof(values["Range"]);
	mMaxDimention = std::stof(values["MaxSize"]);
}


void BlinkAbility::activate(VectorF position)
{
	realiseSize();
	mRect.SetCenter(position);
	mTargetPosition = position;
	mAnimator.selectAnimation("activate");
}

void BlinkAbility::activate(Actor* actor)
{
	BlinkEffect* blink = new BlinkEffect(mTargetPosition);
	mPlayer->addEffect(blink);

	mTimer.start();
}


void BlinkAbility::slowUpdate(float dt)
{
	if (mTimer.getSeconds() > 1.0f)
		mRect.SetSize(mRect.Size() * 0.99);

	if(mRect.Size().x < 10.0f)
		setState(Finished);

	mAnimator.slowUpdate(dt);
	mRect.SetCenter(mTargetPosition);
}
