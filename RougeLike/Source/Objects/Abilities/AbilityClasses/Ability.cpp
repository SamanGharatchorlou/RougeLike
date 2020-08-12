#include "pch.h"
#include "Ability.h"


void Ability::fillBaseValues(const PropertyMap& properties)
{
	mMaxDimention = properties.at(PropertyType::MaxSize);
	mCooldown = Cooldown(properties.at(PropertyType::Cooldown));
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