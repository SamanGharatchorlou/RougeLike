#include "pch.h"
#include "MeleeAbility.h"



void MeleeAbility::activateOn(Actor* target, EffectPool* effectPool)
{
	if (mHitList.count(target) == 0)
	{
		applyEffects(target, effectPool);
		mHitList.insert(target);
	}
}


void MeleeAbility::baseExit()
{
	Ability::baseExit();
	mHitList.clear();
}