#include "pch.h"
#include "SmashAbility.h"

#include "Objects/Effects/DamageEffect.h"
#include "Objects/Effects/StunEffect.h"

#include "Objects/Actors/Player/Player.h"
#include "Graphics/Texture.h"
#include "Animations/Animator.h"
#include "Game/Camera.h"

#include "Events/Events.h"


SmashAbility::SmashAbility(Texture* hammerTexture, VectorF hammerSize, Animator stun) 
	: mHammerTexture(hammerTexture), mStunAnimator(stun), requestedActivate(false), mAppliedDamage(false)
{
	mHammerRect.SetSize(hammerSize);
};


void SmashAbility::fillValues(ValueMap& values)
{
	mDamage = Damage(std::stof(values["Damage"]));
	mMaxDimention = std::stof(values["MaxSize"]);
	mCooldownTime = std::stof(values["Cooldown"]);
	mRange = std::stof(values["Range"]);
	mFallSpeed = std::stof(values["HammerFallSpeed"]);
	mStunSize = realiseSize(mStunAnimator.frameSize(), std::stof(values["StunMaxSize"]));
}


void SmashAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	mHammerRect = mHammerRect.Translate(VectorF(0.0f, mFallSpeed * dt));

	if (hammerHitGround() && requestedActivate == false)
	{
		mAnimator.start();

		ActivateAreaAttack* smashEvent = new ActivateAreaAttack("Smash");
		mEvents.push(EventPacket(smashEvent));

		requestedActivate = true;
	}

	// Completed one animation loop 
	if (mAnimator.loops() > 0)
		mAnimator.stop();

	if (hasCooledDown())
		endAbility();
}



void SmashAbility::activate(VectorF position)
{
	mRect.SetCenter(position);
	mAnimator.selectAnimation(Action::Active);

	float hitPosition = Camera::Get()->toCameraCoords(mRect.Center()).y;
	float fallDistance = Camera::Get()->size().y - hitPosition;

	mHammerRect.SetCenter(mRect.Center() + VectorF(0.0f, -fallDistance));

	beginCooldown();
}


void SmashAbility::activate(Actor* actor)
{
	if (requestedActivate)
	{
		// The enemy state will change to wait (from the stun) before the got hit bool from the
		// damage will change the state to hit. Hence the damage is taken but there is no hit state change
		DamageEffect* damage = new DamageEffect(mDamage);
		actor->addEffect(damage);

		StunEffect* stunEffect = new StunEffect(&mStunAnimator, mStunSize);
		actor->addEffect(stunEffect);
	}
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

		// while hammer is falling
		if (!hammerHitGround())
		{
			RectF hammerRect = Camera::Get()->toCameraCoords(mHammerRect);
			mHammerTexture->render(hammerRect, SDL_FLIP_VERTICAL);
		}
	}
}


void SmashAbility::exit()
{
	mAnimator.stop();
	requestedActivate = false;
	mCooldownTimer.stop();
}



// --- Private Functions --- //
bool SmashAbility::hammerHitGround()
{
	return mHammerRect.BotPoint() >= mRect.BotPoint();
}