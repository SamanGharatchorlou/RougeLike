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
	mCooldownTime = std::stof(values["Cooldown"]);
}


void BlinkAbility::activate(VectorF position)
{
	mRect.SetSize(realiseSize(mAnimator.frameSize(), mMaxDimention));
	mRect.SetCenter(position);
	mTargetPosition = position;
	mAnimator.startAnimation(Action::Active);
}


void BlinkAbility::activate(Actor* actor)
{
	BlinkEffect* blink = new BlinkEffect(mTargetPosition);
	mPlayer->addEffect(blink);
	beginCooldown();
}


void BlinkAbility::slowUpdate(float dt)
{
	if (mAnimator.loops() > 2)
		mRect.SetSize(mRect.Size() * 0.99);

	mAnimator.slowUpdate(dt);
	mRect.SetCenter(mTargetPosition);

	if (hasCooledDown())
		endAbility();
}
