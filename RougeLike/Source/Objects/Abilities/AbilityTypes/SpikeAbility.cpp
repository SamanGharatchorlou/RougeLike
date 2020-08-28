#include "pch.h"
#include "SpikeAbility.h"

#include "Actors/Actor.h"
#include "Objects/Pools/EffectPool.h"



void SpikeAbility::activate(VectorF position)
{
	mRect.SetBotCenter(position);
	mProperties.addXYPosition(position);

	mAnimator.selectAnimation(Action::Active);
	mAnimator.start();

	mActivateCollisions = true;
}


void SpikeAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	if (mAnimator.loops() == 1)
	{
		mCompleted = true;
		mAnimator.stop();
	}
}


void SpikeAbility::applyEffects(Actor* actor, EffectPool* effectPool)
{
	applyEffect(EffectType::Damage, actor, effectPool);
	applyEffect(EffectType::Displacement, actor, effectPool);
}

