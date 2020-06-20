#include "pch.h"
#include "SpikeAbility.h"

#include "Objects/Effects/DisplacementEffect.h"
#include "Objects/Effects/DamageEffect.h"
#include "Objects/Effects/EffectPool.h"

#include "Objects/Actors/Player/Player.h"

#include "Animations/Animator.h"


void SpikeAbility::fillValues(ValueMap& values)
{
	mDamage = Damage(std::stof(values["Damage"]));
	mKnockbackForce = std::stof(values["KnockbackForce"]);
	mKnockbackDistance = std::stof(values["KnockbackDistance"]);

	mMaxDimention = std::stof(values["MaxSize"]);
	mRange = std::stof(values["Range"]);
	mCooldownTime = std::stof(values["Cooldown"]);
}


void SpikeAbility::activate(VectorF position)
{
	mSource = position;
	mRect.SetBotCenter(mSource);
	mAnimator.startAnimation(Action::Active);
}


void SpikeAbility::activate(Actor* actor, EffectPool* effectPool)
{
	Effect* displacement = effectPool->getEffect(EffectType::Displacement);
	DisplacementEffect* displaceEffect = static_cast<DisplacementEffect*>(displacement);
	displaceEffect->set(mSource, mKnockbackForce, mKnockbackDistance);
	actor->addEffect(displacement);

	Effect* damage = effectPool->getEffect(EffectType::Damage);
	DamageEffect* damageEffect = static_cast<DamageEffect*>(damage);
	damageEffect->set(mDamage);
	actor->addEffect(damage);

	beginCooldown();
}


void SpikeAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	// Completed one animation loop
	if (mAnimator.loops() > 0)
		mAnimator.stop();

	if (hasCooledDown())
		endAbility();
}
