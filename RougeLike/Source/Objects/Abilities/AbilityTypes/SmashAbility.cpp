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


void SmashAbility::activate(VectorF position)
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

	// Set displacement source from the throw position
	mProperties.addXYPosition(mCaster->position());

	mActivateCollisions = true;
}


void SmashAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);


	if (!mReachedTarget)
	{
		VectorF velocity = mHammerDirection * mProperties[PropertyType::Velocity] * dt;
		mHammerRect = mHammerRect.Translate(velocity);
		mQuad.translate(velocity);
	}


	// has the direction to the target changed i.e. passed it?
	VectorF currentDirection = (mTargetPosition - mHammerRect.Center()).normalise();
	bool hasPassedTarget = !((currentDirection.x > 0 == mHammerDirection.x > 0) && (currentDirection.y > 0 == mHammerDirection.y > 0));

	// Being explosion
	if (!mReachedTarget && hasPassedTarget)
	{
		mReachedTarget = true;

		mAnimator.start();

		// Final explosion uses mRect not the hammer rect for collision detection
		mQuad = Quad2D<float>(mRect);
		mHitList.clear();
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
	if (mState == AbilityState::Running)
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
}


// --- Private Functions --- //

void SmashAbility::applyEffects(Actor* actor, EffectPool* effectPool)
{
	if (!mReachedTarget)
		applyHammerEffects(actor, effectPool);
	else
		applyExplosionEffects(actor, effectPool);
}


void SmashAbility::applyHammerEffects(Actor* actor, EffectPool* effectPool)
{
	applyEffect(EffectType::Damage, actor, effectPool);
	printf("hammer effect w: %f\n", mQuad[0].x - mQuad[1].x);
}


void SmashAbility::applyExplosionEffects(Actor* actor, EffectPool* effectPool)
{
	applyEffect(EffectType::Damage, actor, effectPool);
	applyEffect(EffectType::Displacement, actor, effectPool);
	applyEffect(EffectType::Stun, actor, effectPool);

	printf("explosion effect w: %f\n", mQuad[0].x - mQuad[1].x);
}