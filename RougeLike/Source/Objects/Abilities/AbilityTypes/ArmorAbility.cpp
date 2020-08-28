#include "pch.h"
#include "ArmorAbility.h"

#include "Actors/Actor.h"
#include "Objects/Pools/EffectPool.h"


void ArmorAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);
	mRect.SetCenter(mCaster->position());

	if (mAnimator.loops() > 4)
		mAnimator.stop();
}


void ArmorAbility::applyEffects(Actor* actor, EffectPool* effectPool)
{
	applyEffect(EffectType::Armor, actor, effectPool);
}