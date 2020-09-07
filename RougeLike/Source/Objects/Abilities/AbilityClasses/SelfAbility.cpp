#include "pch.h"
#include "SelfAbility.h"
#include "Collisions/Colliders/Collider.h"


void SelfAbility::init()
{
	mCollider = new Collider(&mRect);
}


bool SelfAbility::activateOn(Actor* actor, EffectPool* effectPool)
{
	mAnimator.startAnimation(Action::Active);
	applyEffects(mCaster, effectPool);
	playAudio();
	return false;
}
