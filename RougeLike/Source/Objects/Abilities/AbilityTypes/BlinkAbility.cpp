#include "pch.h"
#include "BilnkAbility.h"

#include "Actors/Actor.h"
#include "Objects/Effects/EffectTypes/BlinkEffect.h"
#include "Objects/Effects/EffectPool.h"

#include "Animations/Animator.h"


void BlinkAbility::activateAt(VectorF position, EffectPool* effectPool)
{
	mTargetPosition = position;

	mRect.SetSize(realiseSize(mAnimator.frameSize(), mMaxDimention));
	mRect.SetCenter(mTargetPosition);
	mAnimator.startAnimation(Action::Active);

	applyEffects(effectPool);
}


void BlinkAbility::slowUpdate(float dt)
{
	if (mAnimator.loops() > 2)
		mRect.SetSize(mRect.Size() * 0.99);

	mAnimator.slowUpdate(dt);
	mRect.SetCenter(mTargetPosition);
}


void BlinkAbility::applyEffects(EffectPool* pool)
{
	Effect* effect = pool->getObject(EffectType::Blink);
	BlinkEffect* blinkEffect = static_cast<BlinkEffect*>(effect);
	blinkEffect->set(mTargetPosition);
	mCaster->addEffect(effect);
}