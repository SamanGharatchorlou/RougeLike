#include "pch.h"
#include "TargetAbilities.h"

#include "Game/Camera.h"


void TargetSelfAbility::render()
{
	if (mState == AbilityState::Running)
	{
		RectF rect = Camera::Get()->toCameraCoords(mRect);
		mAnimator.render(rect);
	}
}


void TargetPositionAbility::render()
{
	if (mState == AbilityState::Selected)
	{
		renderRangeCircle();
	}
	else if (mState == AbilityState::Running)
	{
		RectF rect = Camera::Get()->toCameraCoords(mRect);
		mAnimator.render(rect);
	}
}


void TargePositionAttackAbility::render()
{
	if (mState == AbilityState::Selected)
	{
		renderRangeCircle();
	}
	else if (mState == AbilityState::Running)
	{
		RectF rect = Camera::Get()->toCameraCoords(mRect);
		mAnimator.render(rect);
	}
}


void TargePositionAttackAbility::sendActivateOnRequest()
{
	ActivateAreaAttack* smashEvent = new ActivateAreaAttack(this);
	mEvents.push(EventPacket(smashEvent));
}


