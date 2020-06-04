#include "pch.h"
#include "HealAbility.h"

#include "Objects/Actors/Actor.h"
#include "Objects/Properties/PropertyBag.h"
#include "Objects/Effects/HealEffect.h"
#include "Objects/Attributes/Health.h"

#include "Animations/Animator.h"
#include "Game/Camera.h"


void HealAbility::fillValues(ValueMap& values)
{
	mHeal = std::stof(values["Heal"]);
}

void HealAbility::init(Animator animator)
{
	Ability::init(animator);

	mTimer.start();
}

void HealAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);
	mRect.SetCenter(mSelf->position());

	// Completed one animation loop
	if (mTimer.getSeconds() > 2.0f)
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
