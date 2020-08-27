#include "pch.h"
#include "SlashAbility.h"

#include "Actors/Actor.h"

#include "Objects/Effects/EffectTypes/Effect.h"
#include "Objects/Pools/EffectPool.h"

#include "Game/Camera/Camera.h"
#include "Collisions/Colliders/QuadCollider.h"


void SlashAbility::activateAt(VectorF position, EffectPool* effectPool)
{
	// replace regular collider with quad collider
	delete mCollider;
	mCollider = new QuadCollider(&mQuad);

	mTargetDirection = (mCaster->position() - position).normalise();
	mRect.SetLeftCenter(mCaster->position());
	slashOnce();

	mAnimator.startAnimation(Action::Active);
}


void SlashAbility::activateOn(Actor* target, EffectPool* effectPool)
{
	if (mHitList.count(target) == 0)
	{
		applyEffects(target, effectPool);
		mHitList.insert(target);
	}
}


void SlashAbility::applyEffects(Actor* actor, EffectPool* effectPool)
{
	Effect* damage = effectPool->getObject(EffectType::Damage);
	damage->fill(mProperties);
	actor->addEffect(damage);
}


void SlashAbility::fastUpdate(float dt)
{
	mRect.SetLeftCenter(mCaster->rect().RightCenter());
	setQuadRect();

	sendActivateOnRequest();
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

	if (mState == AbilityState::Selected)
	{
		renderRangeCircle();
	}
	else if (mState == AbilityState::Running)
	{
#if DRAW_EFFECT_RECTS
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

	Ability::exit();
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
