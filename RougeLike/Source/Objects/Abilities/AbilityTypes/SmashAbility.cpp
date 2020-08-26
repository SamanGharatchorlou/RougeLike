#include "pch.h"
#include "SmashAbility.h"

#include "Objects/Effects/EffectTypes/DamageEffect.h"
#include "Objects/Effects/EffectTypes/StunEffect.h"
#include "Objects/Pools/EffectPool.h"

#include "Actors/Actor.h"
#include "Graphics/Texture.h"
#include "Animations/Animator.h"
#include "Game/Camera/Camera.h"


SmashAbility::SmashAbility(Texture* hammerTexture, RectF hammerRect) : mHammerTexture(hammerTexture), mHammerRect(hammerRect) { }


void SmashAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	mHammerRect = mHammerRect.Translate(VectorF(0.0f, mProperties[PropertyType::FallSpeed] * dt));

	if (hammerHitGround() && !mAnimator.isRunning())
	{
		mAnimator.start();
		sendActivateOnRequest();
	}

	// Completed one animation loop 
	if (mAnimator.loops() > 0)
	{
		mCompleted = true;
		mAnimator.stop();
	}
}


void SmashAbility::activateAt(VectorF position, EffectPool* pool)
{
	mAnimator.selectAnimation(Action::Active);

	mRect.SetCenter(position);

	float hitPosition = Camera::Get()->toCameraCoords(mRect.Center()).y;
	float fallDistance = Camera::Get()->size().y - hitPosition;
	mHammerRect.SetCenter(mRect.Center() + VectorF(0.0f, -fallDistance));
}


void SmashAbility::activateOn(Actor* actor, EffectPool* effectPool)
{
	applyEffects(actor, effectPool);
}


void SmashAbility::render()
{
	if (mState == AbilityState::Selected)
	{
		renderRangeCircle();
	}
	else if (mState == AbilityState::Running)
	{
		renderAnimator();

		// while hammer is falling
		if (!hammerHitGround())
		{
			RectF hammerRect = Camera::Get()->toCameraCoords(mHammerRect);
			mHammerTexture->render(hammerRect, SDL_FLIP_VERTICAL);
		}
	}
}


// --- Private Functions --- //
bool SmashAbility::hammerHitGround()
{
	return mHammerRect.BotPoint() >= mRect.BotPoint();
}


void SmashAbility::applyEffects(Actor* actor, EffectPool* effectPool)
{
	// The enemy state will change to wait (from the stun) before the got hit bool from the
	// damage will change the state to hit. Hence the damage is taken but there is no hit state change
	applyEffect(EffectType::Damage, actor, effectPool);
	applyEffect(EffectType::Stun, actor, effectPool);
}