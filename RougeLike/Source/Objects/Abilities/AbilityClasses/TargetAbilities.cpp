#include "pch.h"
#include "TargetAbilities.h"


void TargetSelfAbility::render()
{
	if (mState == AbilityState::Running)
	{
		renderAnimator();
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
		renderAnimator();
	}
}


void TargePositionAttackAbility::sendActivateOnRequest()
{
	ActivateAreaAttack* smashEvent = new ActivateAreaAttack(this);
	mEvents.push(EventPacket(smashEvent));
}
