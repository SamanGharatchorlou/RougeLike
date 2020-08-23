#include "pch.h"
#include "Ability.h"

#include "Actors/Actor.h"
#include "Objects/Pools/EffectPool.h"
#include "Objects/Effects/EffectTypes/Effect.h"


void Ability::init(Actor* caster, const PropertyMap& properties, Animator animator)
{
	mCaster = caster;
	mAnimator = animator;
	mProperties = properties;

	mRect.SetSize(realiseSize(animator.frameSize(), properties.at(PropertyType::MaxSize)));
	mCollider.init(&mRect);
	mCooldown = Cooldown(properties.at(PropertyType::Cooldown));

#if NO_ABILITY_COOLDOWNS
	mCooldown = Cooldown(0.0f);
#endif
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



BasicString Ability::name() const
{ 
	AbilityType abilityType = type();
	BasicString id;
	abilityType >> id;

	return id; 
}

