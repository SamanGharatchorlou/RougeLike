#include "pch.h"
#include "SmashAbility.h"

#include "Objects/Actors/Player/Player.h"
#include "Animations/Animator.h"


void SmashAbility::fillValues(ValueMap& values)
{
	mDamage = Damage(std::stof(values["Damage"]));
	mMaxDimention = std::stof(values["MaxSize"]);
	mRange = std::stof(values["Range"]);
}


void SmashAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	// Completed one animation loop
	if (mAnimator.animationIndex() + 1 == mAnimator.animationCount())
		setState(Ability::Finished);
}



void SmashAbility::activate(VectorF position)
{
	mRect.SetCenter(position);

	mAnimator.selectAnimation("activate");
}

