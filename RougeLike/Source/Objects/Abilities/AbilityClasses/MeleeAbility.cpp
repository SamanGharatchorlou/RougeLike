#include "pch.h"
#include "MeleeAbility.h"



bool MeleeAbility::activateOn(Actor* target, EffectPool* effectPool)
{
	if (mHitList.count(target) == 0)
	{
		applyEffects(target, effectPool);
		mHitList.insert(target);
		return true;
	}

	return false;
}


void MeleeAbility::baseExit()
{
	Ability::baseExit();
	mHitList.clear();
}