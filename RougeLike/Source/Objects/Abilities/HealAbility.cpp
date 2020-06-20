#include "pch.h"
#include "HealAbility.h"

#include "Objects/Effects/EffectPool.h"
#include "Objects/Effects/HealEffect.h"

#include "Animations/Animator.h"
#include "Objects/Actors/Player/Player.h"
#include "Objects/Properties/PropertyBag.h"


void HealAbility::fillValues(ValueMap& values)
{
	mMaxDimention = std::stof(values["MaxSize"]);
	mCooldownTime = std::stof(values["Cooldown"]);
	mHeal = Health(std::stof(values["Heal"]));
}

void HealAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	// HACK: added Offset
	mRect.SetBotCenter(mPlayer->rect().BotCenter() + VectorF(0.0f, 10.0f));

	// Completed one animation loop
	if (mAnimator.loops() > 0)
		mAnimator.stop();

	if (hasCooledDown())
		endAbility();
}


void HealAbility::activate(Actor* actor, EffectPool* effectPool)
{
	Effect* effect = effectPool->getEffect(EffectType::Heal);
	HealEffect* healEffect = static_cast<HealEffect*>(effect);
	healEffect->set(mHeal);
	actor->addEffect(effect);

	// TODO: move this to somewhere else?
	Health* hp = static_cast<Health*>(mPlayer->getProperty("Health"));
	SetHealthBarEvent* dataPtr = new SetHealthBarEvent(*hp);
	mEvents.push(EventPacket(dataPtr));

	mAnimator.startAnimation(Action::Active);

	beginCooldown();
}
