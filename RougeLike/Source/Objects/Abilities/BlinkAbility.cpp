#include "pch.h"
#include "BilnkAbility.h"

#include "Objects/Effects/BlinkEffect.h"
#include "Objects/Effects/EffectPool.h"

#include "Objects/Actors/Player/Player.h"

#include "Animations/Animator.h"



void BlinkAbility::fillValues(ValueMap& values)
{
	mRange = std::stof(values["Range"]);
	mMaxDimention = std::stof(values["MaxSize"]);
	mCooldownTime = std::stof(values["Cooldown"]);
}


void BlinkAbility::activate(VectorF position)
{
	mTargetPosition = position;

	mRect.SetSize(realiseSize(mAnimator.frameSize(), mMaxDimention));
	mRect.SetCenter(mTargetPosition);
	mAnimator.startAnimation(Action::Active);
}


void BlinkAbility::activate(Actor* actor, EffectPool* effectPool)
{
	Effect* effect = effectPool->getEffect(EffectType::Blink);
	BlinkEffect* blinkEffect = static_cast<BlinkEffect*>(effect);
	blinkEffect->set(mTargetPosition);
	actor->addEffect(effect);

	beginCooldown();
}


void BlinkAbility::slowUpdate(float dt)
{
	if (mAnimator.loops() > 2)
		mRect.SetSize(mRect.Size() * 0.99);

	mAnimator.slowUpdate(dt);
	mRect.SetCenter(mTargetPosition);

	if (hasCooledDown())
		endAbility();
}
