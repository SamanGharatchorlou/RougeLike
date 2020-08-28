#include "pch.h"
#include "ChargeAbility.h"

#include "Objects/Pools/EffectPool.h"
#include "Actors/Player/Player.h"

#include "Game/Camera/Camera.h"

#include "Collisions/Colliders/QuadCollider.h"


void ChargeAbility::activate(VectorF position)
{
	mDistanceTravelled = 0.0f;

	// set charge target, only use direction() after this!
	VectorF dir = (mCaster->position() - position).normalise();
	mChargeTarget = mCaster->position() + dir * 1000.0f; // any random point far in this direction
	
	mRect.SetCenter(mCaster->position());

	float ratio = 0.6f;
	setScaledQuad(ratio);
	setQuadCollider();


	mAnimator.selectAnimation(Action::Active);

	mWallCollisions.setActor(mCaster);

	setCharging(true);
}


void ChargeAbility::fastUpdate(float dt)
{
	VectorF velocity = direction() * mProperties.at(PropertyType::Force);
	velocity = mWallCollisions.allowedVelocity(mCaster->currentMap(), velocity, dt);

	if(mTimer.getSeconds() < mProperties.at(PropertyType::Time))
	{
		mCaster->physics()->move(velocity, dt);
		mQuad.translate(velocity * dt);
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
}


void ChargeAbility::render()
{
	if (mState == AbilityState::Running)
	{
#if DRAW_ABILITY_RECTS
		debugDrawRect(mRect, RenderColour::Yellow);
		debugDrawPoint(mQuad.rightCenter(), 5.0f, RenderColour::Black); // front point
#endif
#if TRACK_COLLISIONS
		mCollider->renderCollider();
#endif

		RectF renderRect = renderRectFrontToColliderFront(mRect);
		renderRect = Camera::Get()->toCameraCoords(renderRect);

		VectorF aboutPoint = renderRect.Size() / 2.0f;

		mAnimator.render(renderRect, rotation(), aboutPoint);
	}
}


void ChargeAbility::applyEffects(Actor* actor, EffectPool* effectPool)
{
	mProperties.addXYPosition(mCaster->position());

	applyEffect(EffectType::Damage, actor, effectPool);
	applyEffect(EffectType::KnockbackStun, actor, effectPool);
}


void ChargeAbility::exit()
{
	mDistanceTravelled = 0.0f;
	mHitList.clear();
}



// -- Private Functions -- //

void ChargeAbility::setCharging(bool isCharging)
{
	Player* player = static_cast<Player*>(mCaster);
	player->setVisibility(!isCharging);
	player->overrideControl(isCharging);
	mCompleted = !isCharging;
	mActivateCollisions = isCharging;

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


double ChargeAbility::rotation() const
{
	return getRotation(direction()) - 90;
}


// replace regular collider with quad collider
void ChargeAbility::setQuadCollider()
{
	delete mCollider;
	mCollider = new QuadCollider(&mQuad);
}


void ChargeAbility::setScaledQuad(float scale)
{
	RectF scaledRect = mRect;
	scaledRect.SetSize(mRect.Size() * scale);
	scaledRect.SetCenter(mRect.Center());
	mQuad = Quad2D<float>(scaledRect);
	mQuad.rotate(rotation(), scaledRect.Center());
}


RectF ChargeAbility::renderRectFrontToColliderFront(const RectF& renderRect)
{
	Quad2D<float> revertedQuad = mQuad;
	revertedQuad.rotate(-rotation(), revertedQuad.Center());

	// render vs collider rect size difference
	float edgeDistance = mRect.RightPoint() - revertedQuad.rightCenter().x;
	VectorF distance = direction() * edgeDistance;

	// Set rendering rect front end = collider front end
	return mRect.Translate(distance * -0.75f);
}