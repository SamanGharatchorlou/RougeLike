#include "pch.h"
#include "Ability.h"
#include "Objects/Properties/PropertyBag.h"


void Ability::fillAbilityValues(const ValueBag& values)
{
	mMaxDimention = values.get("MaxSize");
	mCooldown = Cooldown(values.get("Cooldown"));
}


void Ability::init(Animator animator, Actor* caster)
{
	mCaster = caster;
	mAnimator = animator;
	mRect.SetSize(realiseSize(animator.frameSize(), mMaxDimention));
}


void Ability::setAnimations(Animator animator)
{
	mAnimator = animator;
	mRect.SetSize(realiseSize(animator.frameSize(), mMaxDimention));
}


void Ability::exit()
{
	mAnimator.reset();
	mCooldown.stop();
}