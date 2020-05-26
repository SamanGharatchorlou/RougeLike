#include "pch.h"
#include "SpikeAbility.h"

#include "Objects/Actors/Actor.h"

#include "Graphics/Texture.h"
#include "Animations/Animator.h"
#include "Game/Camera.h"

#include "Objects/Properties/Property.h"
#include "Objects/Effects/KnockbackEffect.h"
#include "Objects/Effects/DamageEffect.h"


void SpikeAbility::init(Animator animator)
{
	mAnimator = animator;

	// Size
	VectorF baseDimentions = mAnimator.getSpriteTile()->getRect().Size();
	VectorF size = baseDimentions * 1.2f;

	RectF rect(VectorF(-1.0f, -1.0f), size);
	mRect = rect;
}


void SpikeAbility::activate(VectorF position)
{
	mRect.SetBotCenter(position);
	mAnimator.selectAnimation("activate");
}


void SpikeAbility::activate(Actor* actor)
{
	KnockbackEffect* knockback = new KnockbackEffect(mRect.Center(), mForce);
	actor->addEffect(knockback);

	DamageEffect* damage = new DamageEffect(mDamage);
	actor->addEffect(damage);
}


void SpikeAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	// Completed one animation loop
	if (mAnimator.animationIndex() + 1 == mAnimator.animationCount())
		setState(Ability::Finished);
}


void SpikeAbility::render()
{
	RectF rect = Camera::Get()->toCameraCoords(mRect);
	mAnimator.getSpriteTile()->render(rect);
}