#include "pch.h"
#include "SmashAbility.h"

#include "Objects/Actors/Actor.h"
#include "Animations/Animator.h"


void SmashAbility::fillValues(ValueMap& values)
{
	//mHeal = std::stof(values["Heal"]);
	mMaxDimention = std::stof(values["MaxSize"]);
}

void SmashAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	// HACK: added Offset
	mRect.SetBotCenter(mSelf->rect().BotCenter() + VectorF(100.0f, 0.0f));

	// Completed one animation loop
	if (mAnimator.animationIndex() + 1 == mAnimator.animationCount())
		setState(Ability::Finished);
}


void SmashAbility::activate(Actor* actor)
{
	mSelf = actor;

	mAnimator.selectAnimation("activate");
}
