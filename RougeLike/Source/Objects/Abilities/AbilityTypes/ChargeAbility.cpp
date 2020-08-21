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

	VectorF direction = (mCaster->position() - position).normalise();
	mChargeSource = mCaster->position() + direction * 100.0f; // any random point in this particular direction

	setCharging(true);

	mAnimator.startAnimation(Action::Active);
}


void ChargeAbility::activateOn(Actor* target, EffectPool* effectPool)
{
	if (mHitList.count(target) == 0)
	{
		applyEffects(target, effectPool);
		mHitList.insert(target);
	}
}


void ChargeAbility::fastUpdate(float dt)
{
	if (mIsCharging)
	{
		VectorF direction = (mCaster->position() - mChargeSource).normalise();
		VectorF velocity = direction * mProperties.at(PropertyType::Force);
		float movementStep = std::sqrt(velocity.magnitudeSquared());

		if (mDistanceTravelled < mProperties.at(PropertyType::Distance) && canMove(mCaster, velocity, dt))
		{
			mCaster->physics()->move(velocity, dt);
			mDistanceTravelled += movementStep * dt;
		}
		else
		{
			setCharging(false);
			mAnimator.stop();
			mCompleted = true;
		}
	}
}


void ChargeAbility::slowUpdate(float dt)
{
	if (mIsCharging)
	{
		mAnimator.slowUpdate(dt);
		mRect.SetCenter(mCaster->position());

		if (mCaster->collider()->didHit())
		{
			sendActivateOnRequest();
		}
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
		RectF rect = Camera::Get()->toCameraCoords(mRect);

		VectorF direction = (mCaster->position() - mChargeSource).normalise();
		double rotation = getRotation(direction) - 90;

		VectorF aboutPoint = rect.Size() / 2.0f;

		mAnimator.render(rect, rotation, aboutPoint);
	}
}




void ChargeAbility::applyEffects(Actor* actor, EffectPool* effectPool)
{
	Effect* damage = effectPool->getObject(EffectType::Damage);
	damage->fill(mProperties);
	actor->addEffect(damage);

	Effect* displacement = effectPool->getObject(EffectType::KnockbackStun);
	KnockbackStunEffect* displacementEffect = static_cast<KnockbackStunEffect*>(displacement);
	displacementEffect->update(mChargeSource); // TODO: need more testing... charge source or mCaster->postion??
	displacementEffect->fill(mProperties);
	actor->addEffect(displacementEffect);
}


void ChargeAbility::exit()
{
	mChargeSource = VectorF();
	mDistanceTravelled = 0.0f;
	mHitList.clear();

	Ability::exit();
}


void ChargeAbility::setCharging(bool isCharging)
{
	Player* player = static_cast<Player*>(mCaster);
	player->enableBodyCollisions(isCharging);
	player->setVisibility(!isCharging);
	player->overrideControl(isCharging);
	mIsCharging = isCharging;
}