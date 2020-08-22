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

	// Due to not being able to (yet) resolve rotated rects,
	// Just set this to a square to minimise the error at and between rotations
	mRect.SetHeight(mRect.Width());
	mRect.SetSize(mRect.Size() * 0.75f);

	VectorF direction = (mCaster->position() - position).normalise();
	mChargeTarget = mCaster->position() + direction * 1000.0f; // any random point in this particular direction

	mAnimator.selectAnimation(Action::Active);

	setCharging(true);


	//mCaster->rectRef().SetSize(mCaster->rect().Size() * 2.0f);
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
	float movementStep = std::sqrt(velocity.magnitudeSquared());

	if (mDistanceTravelled < mProperties.at(PropertyType::Distance) && canMove(mCaster, velocity, dt))
	{
		mCaster->physics()->move(velocity, dt);
		mDistanceTravelled += movementStep * dt;


		sendActivateOnRequest();
	}
	else
	{
		//setCharging(false);
	}

	if (!canMove(mCaster, velocity, dt))
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
	//else if (mState == AbilityState::Running)
	{
		RectF rect = Camera::Get()->toCameraCoords(mRect);
		rect.SetSize(rect.Size() / 0.75f);
		double rotation = getRotation(direction()) - 90;
		VectorF aboutPoint = rect.Size() / 2.0f;

#if DRAW_EFFECT_RECTS
		debugDrawRect(mRect, RenderColour::Yellow);
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


	//mCaster->rectRef().SetSize(mCaster->rect().Size() / 2.0f);
}


void ChargeAbility::setCharging(bool isCharging)
{
	Player* player = static_cast<Player*>(mCaster);
	//player->enableBodyCollisions(isCharging);
	player->setVisibility(!isCharging);
	player->overrideControl(isCharging);
	mCompleted = !isCharging;

	isCharging ? mAnimator.start() : mAnimator.stop();
}


VectorF ChargeAbility::direction() const
{
	return (mCaster->position() - mChargeTarget).normalise();
}