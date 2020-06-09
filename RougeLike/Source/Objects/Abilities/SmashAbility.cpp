#include "pch.h"
#include "SmashAbility.h"

#include "Objects/Actors/Player/Player.h"
#include "Graphics/Texture.h"
#include "Animations/Animator.h"
#include "Game/Camera.h"



SmashAbility::SmashAbility(Texture* hammerTexture, VectorF hammerSize) : mHammerTexture(hammerTexture) 
{
	mHammerRect.SetSize(hammerSize);
};


void SmashAbility::fillValues(ValueMap& values)
{
	mDamage = Damage(std::stof(values["Damage"]));
	mMaxDimention = std::stof(values["MaxSize"]);
	mRange = std::stof(values["Range"]);
}


void SmashAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	mHammerRect = mHammerRect.Translate(VectorF(0.0f, mFallSpeed * dt));

	if (mHammerRect.BotPoint() >= mRect.Center().y)
	{
		mAnimator.start();
		mHammerRect.SetTopLeft(VectorF(-1000.0f, -1000.0f)); // Move it out of screen
	}

	// Completed one animation loop 
	if (mAnimator.loops() > 0)
		setState(Ability::Finished);
}



void SmashAbility::activate(VectorF position)
{
	mRect.SetCenter(position);
	mAnimator.selectAnimation(Action::Active);

	float hitPosition = Camera::Get()->toCameraCoords(mRect.Center()).y;
	float fallDistance = Camera::Get()->size().y - hitPosition;

	mHammerRect.SetCenter(mRect.Center() + VectorF(0.0f, -fallDistance));

	float animationTime = mAnimator.frameTime() * mAnimator.frameCount();
	mFallSpeed = (fallDistance / animationTime) * 1.5f;
}


void SmashAbility::render()
{
	// Range circle
	if (mState == Selected)
	{
		VectorF position = Camera::Get()->toCameraCoords(mPlayer->position());
		VectorF size = VectorF(mRange, mRange) * 2.0f;

		RectF rect = RectF(VectorF(), size);
		rect.SetCenter(position);

		mRangeCircle->render(rect);
	}
	else if (mState == Running)
	{
		if(mAnimator.isRunning())
			Ability::render();

		RectF hammerRect = Camera::Get()->toCameraCoords(mHammerRect);
		mHammerTexture->render(hammerRect, SDL_FLIP_VERTICAL);
	}
}


void SmashAbility::exit()
{
	mAnimator.stop();
}