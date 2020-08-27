#include "pch.h"
#include "Ability.h"

#include "Actors/Actor.h"
#include "Objects/Pools/EffectPool.h"
#include "Objects/Effects/EffectTypes/Effect.h"


#include "Game/Camera/Camera.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif


Ability::~Ability()
{
	delete mCollider;
}


void Ability::init(Actor* caster, const PropertyMap& properties, Animator animator)
{
	mCaster = caster;
	mAnimator = animator;
	mProperties = properties;

	mRect.SetSize(realiseSize(animator.frameSize(), properties.at(PropertyType::MaxSize)));
	mCollider = new Collider(&mRect);
	mCooldown = Cooldown(properties.at(PropertyType::Cooldown));

#if NO_ABILITY_COOLDOWNS
	mCooldown = Cooldown(0.0f);
#endif
}


void Ability::baseExit()
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


void Ability::renderAnimator()
{
#if DRAW_ABILITY_RECTS
	debugDrawRect(mRect, RenderColour::Yellow);
#endif
#if TRACK_COLLISIONS
	mCollider->renderCollider();
#endif

	mAnimator.render(Camera::Get()->toCameraCoords(mRect));


}