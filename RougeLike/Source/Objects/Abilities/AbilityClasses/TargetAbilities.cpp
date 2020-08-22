#include "pch.h"
#include "TargetAbilities.h"

#include "Game/Camera/Camera.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif


void TargetSelfAbility::render()
{
	if (mState == AbilityState::Running)
	{
		animRender(&mAnimator, mRect);
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
		animRender(&mAnimator, mRect);
	}
}


void TargePositionAttackAbility::sendActivateOnRequest()
{
	ActivateAreaAttack* smashEvent = new ActivateAreaAttack(this);
	mEvents.push(EventPacket(smashEvent));
}



void animRender(Animator* animator, const RectF& rect)
{
#if DRAW_EFFECT_RECTS
	debugDrawRect(rect, RenderColour::Yellow);
#endif

	animator->render(Camera::Get()->toCameraCoords(rect));
}