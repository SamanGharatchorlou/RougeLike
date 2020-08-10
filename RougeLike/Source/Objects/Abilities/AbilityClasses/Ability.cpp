#include "pch.h"
#include "Ability.h"
#include "Objects/Bags/PropertyBag.h"


void Ability::fillBaseValues(const PropertyBag& properties)
{
	mMaxDimention = properties.get(PropertyType::MaxSize);
	mCooldown = Cooldown(properties.get(PropertyType::Cooldown));
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