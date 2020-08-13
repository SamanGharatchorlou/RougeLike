#include "pch.h"
#include "SpikeAbility.h"

#include "Actors/Actor.h"

#include "Objects/Effects/EffectTypes/Effect.h"
#include "Objects/Pools/EffectPool.h"



void SpikeAbility::activateAt(VectorF position, EffectPool* effectPool)
{
	// TODO: dont need mSource, just set it here?
	mSource = position;
	mRect.SetBotCenter(mSource);
	sendActivateOnRequest();

	mAnimator.startAnimation(Action::Active);
}


void SpikeAbility::activateOn(Actor* actor, EffectPool* effectPool)
{
	applyEffects(actor, effectPool);
}


void SpikeAbility::applyEffects(Actor* actor, EffectPool* effectPool)
{
	Effect* displacement = effectPool->getObject(EffectType::Displacement);
	displacement->fill(mProperties);

	Effect* damage = effectPool->getObject(EffectType::Damage);
	damage->fill(mProperties);

	actor->addEffect(damage);
	actor->addEffect(displacement);
}



void SpikeAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	if (mAnimator.loops() > 0)
		mAnimator.stop();
}
