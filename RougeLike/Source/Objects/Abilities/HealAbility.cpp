#include "pch.h"
#include "HealAbility.h"

#include "Objects/Actors/Actor.h"
#include "Objects/Properties/PropertyBag.h"
#include "Objects/Effects/HealEffect.h"
#include "Objects/Attributes/Health.h"

#include "Animations/Animator.h"


void HealAbility::fillValues(ValueMap& values)
{
	mHeal = std::stof(values["Heal"]);
	mMaxDimention = std::stof(values["MaxSize"]);
}

void HealAbility::init(Animator animator)
{
	Ability::init(animator);
	mTimer.start();
}

void HealAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	// HACK: added Offset
	mRect.SetBotCenter(mSelf->rect().BotCenter() + VectorF(0.0f, 10.0f));

	// Completed one animation loop
	if (mAnimator.animationIndex() + 1 == mAnimator.animationCount())
		setState(Ability::Finished);
}


void HealAbility::activate(Actor* actor)
{
	mSelf = actor;

	HealEffect* healEffect = new HealEffect(mHeal);
	mSelf->addEffect(healEffect);

	Health* hp = static_cast<Health*>(mSelf->getProperty("Health"));
	SetHealthBarEvent* dataPtr = new SetHealthBarEvent(*hp);
	mEvents.push(EventPacket(dataPtr));

	mAnimator.selectAnimation("activate");
	mTimer.restart();
}
