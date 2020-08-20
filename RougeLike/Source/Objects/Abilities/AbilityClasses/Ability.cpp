#include "pch.h"
#include "Ability.h"


void Ability::init(const BasicString& name, Actor* caster, const PropertyMap& properties, Animator animator)
{
	mName = name;
	mCaster = caster;
	mAnimator = animator;
	mProperties = properties;

	mRect.SetSize(realiseSize(animator.frameSize(), properties.at(PropertyType::MaxSize)));
	mCooldown = Cooldown(properties.at(PropertyType::Cooldown));
}


void Ability::exit()
{
	mAnimator.reset();
	mCooldown.stop();
	mCompleted = false;
}