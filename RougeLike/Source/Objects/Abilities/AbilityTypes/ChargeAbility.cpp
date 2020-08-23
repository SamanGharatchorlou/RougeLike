#include "pch.h"
#include "ChargeAbility.h"

#include "Objects/Effects/EffectTypes/Effect.h"
#include "Objects/Effects/EffectTypes/KnockbackStunEffect.h"
#include "Objects/Pools/EffectPool.h"
#include "Actors/Player/Player.h"

#include "Game/Camera/Camera.h"



void ChargeAbility::activateAt(VectorF position, EffectPool* effectPool)
{
	mDistanceTravelled = 0.0f;

	// Square with a width of ratio * original width
	float ratio = 0.45f;
	VectorF scale = VectorF(1.0f, mRect.Width() / mRect.Height()) * ratio;
	mCollider.init(&mRect, scale);

	VectorF direction = (mCaster->position() - position).normalise();
	mChargeTarget = mCaster->position() + direction * 1000.0f; // any random point in this particular direction

	mAnimator.selectAnimation(Action::Active);

	mWallCollisions.setActor(mCaster);

	setCharging(true);
}


void ChargeAbility::activateOn(Actor* target, EffectPool* effectPool)
{
	if (mHitList.count(target) == 0)
	{
		mProperties.addXYPosition(mCaster->position());
		applyEffects(target, effectPool);
		mHitList.insert(target);
	}
}


void ChargeAbility::fastUpdate(float dt)
{
	VectorF velocity = direction() * mProperties.at(PropertyType::Force);
	velocity = mWallCollisions.allowedVelocity(mCaster->currentMap(), velocity, dt);

	if(mTimer.getSeconds() < mProperties.at(PropertyType::Time))
	{
		mCaster->physics()->move(velocity, dt);
		sendActivateOnRequest();
	}
	else
	{
		setCharging(false);
	}
}


void ChargeAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);
	mRect.SetCenter(mCaster->position());

	if (mCaster->collider()->didHit())
	{
		sendActivateOnRequest();
	}
}


void ChargeAbility::render()
{

	if (mState == AbilityState::Selected)
	{
		renderRangeCircle();
	}
	else if (mState == AbilityState::Running)
	{
		RectF collRect = mCollider.scaledRect();

		// render vs collider rect size difference
		float edgeDistance = mRect.RightPoint() - collRect.RightPoint();
		VectorF distance = direction() * edgeDistance;

		// Set rendering rect front end = collider front end
		RectF rect = mRect.Translate(distance * -1.0f);

		rect = Camera::Get()->toCameraCoords(rect);

		double rotation = getRotation(direction()) - 90;
		VectorF aboutPoint = rect.Size() / 2.0f;

#if DRAW_EFFECT_RECTS
		debugDrawRect(mRect, RenderColour::Yellow);
		debugDrawRect(mCollider.scaledRect(), RenderColour::Red);
#endif

		mAnimator.render(rect, rotation, aboutPoint);
	}

}


void ChargeAbility::applyEffects(Actor* actor, EffectPool* effectPool)
{
	applyEffect(EffectType::Damage, actor, effectPool);
	applyEffect(EffectType::KnockbackStun, actor, effectPool);
}


void ChargeAbility::exit()
{
	mChargeTarget = VectorF();
	mDistanceTravelled = 0.0f;
	mHitList.clear();

	Ability::exit();
}


void ChargeAbility::setCharging(bool isCharging)
{
	Player* player = static_cast<Player*>(mCaster);
	player->setVisibility(!isCharging);
	player->overrideControl(isCharging);
	mCompleted = !isCharging;

	if (isCharging)
	{
		mTimer.start();
		mAnimator.start();
	}
	else
	{
		mTimer.stop();
		mAnimator.stop();
	}
}


VectorF ChargeAbility::direction() const
{
	return (mCaster->position() - mChargeTarget).normalise();
}