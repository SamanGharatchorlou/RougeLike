#include "pch.h"
#include "HealAbility.h"

#include "Actors/Actor.h"

#include "Objects/Effects/EffectTypes/Effect.h"
#include "Objects/Pools/EffectPool.h"

#include "Collisions/Collider.h"


void HealAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);
	mRect.SetBotCenter(mCaster->collider()->scaledRect().BotCenter() + VectorF(0.0f, 10.0f));

	// Completed one animation loop
	if (mAnimator.loops() > 0)
	{
		mAnimator.stop();
		mCompleted = true;
	}
}


void HealAbility::activate(EffectPool* pool)
{
	mAnimator.startAnimation(Action::Active);

	applyEffects(pool);
	//updateUI();
}


void HealAbility::applyEffects(EffectPool* pool)
{
	Effect* effect = pool->getObject(EffectType::Heal);
	effect->fill(mProperties);
	mCaster->addEffect(effect);
}

// TODO: this happens too early due to the delayed add from the health effect
void HealAbility::updateUI()
{
	Health* hp = static_cast<Health*>(mCaster->getAttribute(AttributeType::Health));
	SetUIBarEvent* healthBar = new SetUIBarEvent("HealthBar", "BlackHealthBar", hp->getPercentage());
	mEvents.push(EventPacket(healthBar));
}