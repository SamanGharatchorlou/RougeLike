#include "pch.h"
#include "SmashAbility.h"

#include "Objects/Effects/EffectTypes/DamageEffect.h"
#include "Objects/Effects/EffectTypes/StunEffect.h"
#include "Objects/Pools/EffectPool.h"

#include "Actors/Actor.h"
#include "Graphics/Texture.h"
#include "Animations/Animator.h"
#include "Game/Camera/Camera.h"

#include "Collisions/Colliders/QuadCollider.h"


// TODO: fix the placement of this, now that its large its hard to place
SmashAbility::SmashAbility(Texture* hammerTexture, RectF hammerRect) : mHammerTexture(hammerTexture), mHammerRect(hammerRect) { }


void SmashAbility::activateAt(VectorF position, EffectPool* pool)
{
	// Splash animation
	mAnimator.selectAnimation(Action::Active);
	mRect.SetCenter(position);

	// replace regular collider with quad colliderd
	delete mCollider;
	mCollider = new QuadCollider(&mQuad);

	// Hammer logic
	mTargetPosition = position;
	mHammerDirection = (mTargetPosition - mCaster->position()).normalise();

	mHammerRect.SetCenter(mCaster->position());

	// set quad
	mQuad = Quad2D<float>(mHammerRect);
	mQuad.rotate(getRotation(mHammerDirection), mHammerRect.Center());
}


void SmashAbility::activateOn(Actor* actor, EffectPool* effectPool)
{
	if (!mReachedTarget)
	{
		if (mHitList.count(actor) == 0)
		{
			applyHammerEffects(actor, effectPool);
		}
	}
	else
	{
		applyExplosionEffects(actor, effectPool);
	}
}


void SmashAbility::fastUpdate(float dt)
{
	// Flying hanner
	if (!mReachedTarget)
		sendActivateOnRequest(); 
}


void SmashAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	VectorF velocity = mHammerDirection * mProperties[PropertyType::Velocity] * dt;
	mHammerRect = mHammerRect.Translate(velocity);
	mQuad.translate(velocity);


	// Same direction to the target?
	VectorF currentDirection = (mTargetPosition - mHammerRect.Center()).normalise();
	bool hasPassedTarget = !((currentDirection.x > 0 == mHammerDirection.x > 0) && (currentDirection.y > 0 == mHammerDirection.y > 0));

	if (!mReachedTarget && hasPassedTarget)
	{
		mReachedTarget = true;
	}

	// Final explosions
	if (mReachedTarget && !mAnimator.isRunning())
	{
		mAnimator.start();

		// Final explosion uses mRect not the hammer rect for collision detection
		mQuad = Quad2D<float>(mRect);
		sendActivateOnRequest();
	}

	// Completed one animation loop 
	if (mAnimator.loops() > 0)
	{
		mCompleted = true;
		mAnimator.stop();
	}
}


void SmashAbility::render()
{
	if (mState == AbilityState::Selected)
	{
		renderRangeCircle();
	}
	else if (mState == AbilityState::Running)
	{
#if DRAW_ABILITY_RECTS
		debugDrawRect(mRect, RenderColour::Yellow);
		debugDrawRect(mHammerRect, RenderColour::Yellow);
		debugDrawLine(mHammerRect.Center(), mTargetPosition, RenderColour::Black);
#endif
#if TRACK_COLLISIONS
		mCollider->renderCollider();
#endif

		RectF renderRect = mRect; Camera::Get()->toCameraCoords(mRect);
		renderRect.SetSize(mRect.Size() * 1.5f);
		renderRect.SetCenter(mRect.Center());
		renderRect = Camera::Get()->toCameraCoords(renderRect);

		mAnimator.render(renderRect);

		// while hammer is flying
		if (!mReachedTarget)
		{
			RectF hammerRect = Camera::Get()->toCameraCoords(mHammerRect);
			VectorF aboutPoint = hammerRect.Size() / 2.0f;
			mHammerTexture->render(hammerRect, getRotation(mHammerDirection), aboutPoint);
		}
	}
}

void SmashAbility::exit()
{
	mReachedTarget = false;
	mHitList.clear();
}


// --- Private Functions --- //
void SmashAbility::applyHammerEffects(Actor* actor, EffectPool* effectPool)
{
	applyEffect(EffectType::Damage, actor, effectPool);
	mHitList.insert(actor);
}


void SmashAbility::applyExplosionEffects(Actor* actor, EffectPool* effectPool)
{
	mProperties.addXYPosition(mHammerRect.Center());

	applyEffect(EffectType::Damage, actor, effectPool);
	applyEffect(EffectType::Displacement, actor, effectPool);
	applyEffect(EffectType::Stun, actor, effectPool);
}