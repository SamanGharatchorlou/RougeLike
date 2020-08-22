#include "pch.h"
#include "SpikeAbility.h"

#include "Actors/Actor.h"

#include "Objects/Effects/EffectTypes/Effect.h"
#include "Objects/Pools/EffectPool.h"



void SpikeAbility::activateAt(VectorF position, EffectPool* effectPool)
{
	mRect.SetBotCenter(position);
	mProperties.addXYPosition(position);
	sendActivateOnRequest();

	mAnimator.selectAnimation(Action::Active);
	mAnimator.start();
}


void SpikeAbility::activateOn(Actor* actor, EffectPool* effectPool)
{
	applyEffects(actor, effectPool);
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