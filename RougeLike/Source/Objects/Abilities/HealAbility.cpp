#include "pch.h"
#include "HealAbility.h"

#include "Objects/Actors/Player/Player.h"
#include "Objects/Properties/PropertyBag.h"
#include "Objects/Effects/HealEffect.h"
#include "Objects/Attributes/Health.h"

#include "Animations/Animator.h"


void HealAbility::fillValues(ValueMap& values)
{
	mHeal = std::stof(values["Heal"]);
	mMaxDimention = std::stof(values["MaxSize"]);
	mCooldownTime = std::stof(values["Cooldown"]);
}

void HealAbility::init(Animator animator, Player* player)
{
	Ability::init(animator, player);

	beginCooldown();
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


void HealAbility::activate(Actor* actor)
{
	HealEffect* healEffect = new HealEffect(mHeal);
	mPlayer->addEffect(healEffect);

	Health* hp = static_cast<Health*>(mPlayer->getProperty("Health"));
	SetHealthBarEvent* dataPtr = new SetHealthBarEvent(*hp);
	mEvents.push(EventPacket(dataPtr));

	mAnimator.startAnimation(Action::Active);
}
