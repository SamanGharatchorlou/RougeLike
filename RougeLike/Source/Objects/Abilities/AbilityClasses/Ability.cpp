#include "pch.h"
#include "Ability.h"

#include "Actors/Actor.h"
#include "Objects/Pools/EffectPool.h"
#include "Objects/Effects/EffectTypes/Effect.h"


void Ability::init(const BasicString& name, Actor* caster, const PropertyMap& properties, Animator animator)
{
	mName = name;
	mCaster = caster;
	mAnimator = animator;
	mProperties = properties;

	mRect.SetSize(realiseSize(animator.frameSize(), properties.at(PropertyType::MaxSize)));
	mCollider.init(&mRect);
	mCooldown = Cooldown(properties.at(PropertyType::Cooldown));
}


void Ability::exit()
{
	mAnimator.reset();
	mCooldown.stop();
	mCompleted = false;
}


void Ability::applyEffect(EffectType effectType, Actor* target, EffectPool* effectPool) const
{
	Effect* effect = effectPool->getObject(effectType);
	effect->fill(mProperties);
	target->addEffect(effect);
}