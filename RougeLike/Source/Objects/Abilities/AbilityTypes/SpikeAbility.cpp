#include "pch.h"
#include "SpikeAbility.h"

#include "Objects/Actors/Actor.h"

#include "Objects/Effects/EffectPool.h"
#include "Objects/Effects/DisplacementEffect.h"
#include "Objects/Effects/DamageEffect.h"


void SpikeAbility::fillValues(ValueMap& values)
{
	mDamage = Damage(std::stof(values["Damage"]));
	mKnockbackForce = std::stof(values["KnockbackForce"]);
	mKnockbackDistance = std::stof(values["KnockbackDistance"]);
}


void SpikeAbility::activateAt(VectorF position, EffectPool* effectPool)
{
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
	Effect* displacement = effectPool->getEffect(EffectType::Displacement);
	DisplacementEffect* displaceEffect = static_cast<DisplacementEffect*>(displacement);
	displaceEffect->set(mSource, mKnockbackForce, mKnockbackDistance);
	actor->addEffect(displacement);

	Effect* damage = effectPool->getEffect(EffectType::Damage);
	DamageEffect* damageEffect = static_cast<DamageEffect*>(damage);
	damageEffect->set(mDamage);
	actor->addEffect(damage);
}



void SpikeAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	if (mAnimator.loops() > 0)
		mAnimator.stop();
}
