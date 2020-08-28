#include "pch.h"
#include "SlashAbility.h"

#include "Actors/Actor.h"

#include "Objects/Pools/EffectPool.h"

#include "Game/Camera/Camera.h"
#include "Collisions/Colliders/QuadCollider.h"


void SlashAbility::activate(VectorF posision)
{
	mTargetDirection = (mCaster->position() - posision).normalise();
	mAnimator.startAnimation(Action::Active);

	// replace regular collider with quad collider
	delete mCollider;
	mCollider = new QuadCollider(&mQuad);

	mRect.SetLeftCenter(mCaster->position());
	slashOnce();

	mActivateCollisions = true;
}


void SlashAbility::applyEffects(Actor* actor, EffectPool* effectPool)
{
	applyEffect(EffectType::Damage, actor, effectPool);
}


void SlashAbility::fastUpdate(float dt)
{
	mRect.SetLeftCenter(mCaster->rect().RightCenter());
	setQuadRect();
}


void SlashAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	if (mAnimator.loops() == 1)
	{
		slashOnce();
	}
	else if (mAnimator.loops() > 1)
	{
		mAnimator.stop();
		mCompleted = true;
	}
}


void SlashAbility::render()
{
	if (mState == AbilityState::Running)
	{
#if DRAW_ABILITY_RECTS
		debugDrawRect(mRect, RenderColour::Yellow);
		debugDrawLine(mCaster->position(), mCaster->position() + mTargetDirection * 10.0, RenderColour::Black);
#endif
#if TRACK_COLLISIONS
		mCollider->renderCollider();
#endif

		RectF renderRect = Camera::Get()->toCameraCoords(mRect);
		VectorF aboutPoint = VectorF(-mCaster->rect().Width(), mRect.Size().y) / 2.0f;

		mAnimator.render(renderRect, getRotation(mTargetDirection) + 90, aboutPoint);
	}

}


void SlashAbility::exit()
{
	mRect.SetSize(mRect.Size() / 2.0f);
	mSlashCount = 0;
}


// -- Private functions -- //
void SlashAbility::slashOnce()
{
	if (mSlashCount < 2)
	{
		if (mSlashCount == 1)
		{
			mRect.SetSize(mRect.Size() * 2.0f);
			mRect.SetLeftCenter(mCaster->rect().RightCenter());
		}

		setQuadRect();

		mSlashCount++;
		mHitList.clear();
	}
}


void SlashAbility::setQuadRect()
{
	mQuad = Quad2D<float>(mRect);
	mQuad.rotate(getRotation(mTargetDirection) + 90, mCaster->position());
}
