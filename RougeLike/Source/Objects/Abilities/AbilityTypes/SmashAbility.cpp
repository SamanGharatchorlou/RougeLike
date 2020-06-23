#include "pch.h"
#include "SmashAbility.h"

#include "Objects/Effects/DamageEffect.h"
#include "Objects/Effects/StunEffect.h"
#include "Objects/Effects/EffectPool.h"

#include "Objects/Actors/Player/Player.h"
#include "Graphics/Texture.h"
#include "Animations/Animator.h"
#include "Game/Camera.h"

#include "Events/Events.h"


SmashAbility::SmashAbility(Texture* hammerTexture, VectorF hammerSize) 
	: mHammerTexture(hammerTexture), requestedActivate(false), mAppliedDamage(false), mFallSpeed(0.0f), mTime(0.0f)
{
	mHammerRect.SetSize(hammerSize);
};


void SmashAbility::fillValues(ValueMap& values)
{
	mMaxDimention = std::stof(values["MaxSize"]);
	mCooldownTime = std::stof(values["Cooldown"]);
	mRange = std::stof(values["Range"]);

	mFallSpeed = std::stof(values["HammerFallSpeed"]);

	mDamage = Damage(std::stof(values["Damage"]));
	mTime = std::stof(values["StunTime"]);
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


void SmashAbility::activate(Actor* actor, EffectPool* effectPool)
{
	if (requestedActivate)
	{
		// The enemy state will change to wait (from the stun) before the got hit bool from the
		// damage will change the state to hit. Hence the damage is taken but there is no hit state change
		Effect* damage = effectPool->getEffect(EffectType::Damage);
		DamageEffect* damageEffect = static_cast<DamageEffect*>(damage);
		damageEffect->set(mDamage);
		actor->addEffect(damageEffect);

		Effect* stun = effectPool->getEffect(EffectType::Stun);
		StunEffect* stunEffect = static_cast<StunEffect*>(stun);
		stunEffect->set(mTime);
		actor->addEffect(stunEffect);
	}
}


void SmashAbility::render()
{
	// Range circle
	if (mState == Selected)
	{
		VectorF position = Camera::Get()->toCameraCoords(mCaster->position());
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