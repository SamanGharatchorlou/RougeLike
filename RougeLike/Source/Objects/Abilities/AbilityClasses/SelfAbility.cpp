#include "pch.h"
#include "SelfAbility.h"


bool SelfAbility::activateOn(Actor* actor, EffectPool* effectPool)
{
	mAnimator.startAnimation(Action::Active);
	applyEffects(mCaster, effectPool);
	mCompleted = true;
	return false;
}