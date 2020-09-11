#include "pch.h"
#include "SelfAbility.h"
#include "Collisions/Colliders/Collider.h"


void SelfAbility::init()
{
	mCollider = new Collider(&mRect);
}


bool SelfAbility::activateOn(Actor* actor, EffectPool* effectPool)
{
	mAnimator.startAnimation(Animation::Active);
	applyEffects(mCaster, effectPool);
	playAudio();
	return false;
}
