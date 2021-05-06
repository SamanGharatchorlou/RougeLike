#include "pch.h"
#include "SlashAbility.h"

#include "Game/Camera/Camera.h"
#include "Audio/AudioManager.h"
#include "Input/InputManager.h"
#include "Objects/Pools/EffectPool.h"

#include "Actors/Actor.h"
#include "Collisions/Colliders/QuadCollider.h"




void SlashAbility::handleInput(const InputManager* input)
{
	VectorF cameraPosition = Camera::Get()->rect().TopLeft();
	VectorF cursorPos = input->cursorPosition() + cameraPosition;
	mTargetDirection = (mCaster->position() - cursorPos).normalise();

	updateQuad();
}


void SlashAbility::activate()
{
	mAnimator.startAnimation(Animation::Active);

	slashOnce();

	mActivateCollisions = true;
}


void SlashAbility::applyEffects(Actor* actor, EffectPool* effectPool)
{
	applyEffect(EffectType::Damage, actor, effectPool);
}


void SlashAbility::fastUpdate(float dt)
{
	updateQuad();
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
		Quad2D<float> largeQuad = Camera::Get()->toCameraCoords(mQuad);
		renderQuadOutline(largeQuad, mSelectHighlight);

		RectF rect = mRect;
		rect.SetSize(mRect.Size() / 2.0f);
		rect.SetLeftCenter(mCaster->rect().RightCenter());

		Quad2D<float> smallerQuad = Quad2D<float>(rect);
		smallerQuad.rotate(getRotation(mTargetDirection) + 180, mCaster->position());
		smallerQuad = Camera::Get()->toCameraCoords(smallerQuad);
		renderQuadOutline(smallerQuad, mSelectHighlight);
	}
	else if (mState == AbilityState::Running)
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

		mAnimator.render(renderRect, getRotation(mTargetDirection) + 180, aboutPoint);
	}
}


void SlashAbility::exit()
{
	mSlashCount = 0;
}


// -- Private functions -- //
void SlashAbility::slashOnce()
{
	if (mSlashCount < 2)
	{
		mSlashCount++;

		VectorF size = mRect.Size();
		if (mSlashCount == 1)
		{
			AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::Play, "Slash1", mCaster));
			size /= 2.0f;
			Camera::Get()->getShake()->addTrauma(50);
		}
		else if (mSlashCount == 2)
		{
			AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::Play, "Slash2", mCaster));
			size *= 2.0f;
			Camera::Get()->getShake()->addTrauma(80);
		}

		mRect.SetSize(size);
		mRect.SetLeftCenter(mCaster->rect().RightCenter());

		updateQuad();
		mHitList.clear();

	}
}


void SlashAbility::updateQuad()
{
	mRect.SetLeftCenter(mCaster->rect().RightCenter());
	mQuad = Quad2D<float>(mRect);
	mQuad.rotate(getRotation(mTargetDirection) + 180, mCaster->position());
}
