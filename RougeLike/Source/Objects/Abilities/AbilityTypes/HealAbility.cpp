#include "pch.h"
#include "HealAbility.h"

#include "Actors/Player/Player.h"
#include "Objects/Effects/EffectPool.h"
#include "Objects/Effects/EffectTypes/HealEffect.h"

#include "Collisions/Collider.h"


void HealAbility::fillValues(const ValueBag& values)
{
	Ability::fillValues(values);
	mHeal = Health(values.get("Heal"));
}


void HealAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);
	mRect.SetBotCenter(mCaster->collider()->scaledRect().BotCenter() + VectorF(0.0f, 10.0f));

	// Completed one animation loop
	if (mAnimator.loops() > 0)
	{

		mAnimator.stop();
	}
}


void HealAbility::activate(EffectPool* pool)
{
	mAnimator.startAnimation(Action::Active);

	applyEffects(pool);
	updateUI();
}


void HealAbility::applyEffects(EffectPool* pool)
{
	Effect* effect = pool->getObject(EffectType::Heal);
	HealEffect* healEffect = static_cast<HealEffect*>(effect);
	healEffect->set(mHeal);
	mCaster->addEffect(effect);
}


void HealAbility::updateUI()
{
	Player* player = dynamic_cast<Player*>(mCaster);

	if (mCaster)
	{
		Health* hp = static_cast<Health*>(mCaster->getAttribute(AttributeType::Health));
		SetHealthBarEvent* hpPtr = new SetHealthBarEvent(*hp);
		mEvents.push(EventPacket(hpPtr));
	}
}