#include "pch.h"
#include "ChargeAbility.h"

#include "Audio/AudioManager.h"
#include "Input/InputManager.h"
#include "Game/Camera/Camera.h"

#include "Objects/Pools/EffectPool.h"
#include "Collisions/Colliders/QuadCollider.h"
#include "Actors/Player/Player.h"
#include "Objects/Properties/Attributes/Health.h"


ChargeAbility::ChargeAbility()
{
	mSelectionCollider.init(&mSelectionQuad);
};

void ChargeAbility::handleInput(const InputManager* input)
{
	VectorF cameraPosition = Camera::Get()->rect().TopLeft();
	VectorF cursorPos = input->cursorPosition() + cameraPosition;

	VectorF dir = (mCaster->position() - cursorPos);
	mChargeTarget = mCaster->position() + dir * 1000.0f; // any random point far along in this direction
	
	updateSelectionQuad();
}


void ChargeAbility::activate()
{
	updateQuad();

	mAnimator.selectAnimation(Animation::Active);
	mWallCollisions.setActor(mCaster);
	setCharging(true);

	Camera::Get()->getShake()->addTrauma(90);
}


void ChargeAbility::fastUpdate(float dt)
{
	VectorF velocity = direction() * mProperties.at(PropertyType::Velocity);
	velocity = mWallCollisions.allowedVelocity(mCaster->currentMap(), velocity, dt);

	if(mTimer.getSeconds() < mProperties.at(PropertyType::Time))
	{
		mCaster->physics()->move(velocity, dt);
	}
	else
	{
		setCharging(false);
	}

	updateQuad();
}


void ChargeAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	if (mTimer.getSeconds() < mProperties.at(PropertyType::Time) * 0.7f)
	{
		CameraShake* shakeyCam = Camera::Get()->getShake();
		float maintenanceTrauma = shakeyCam->reductionRate() * dt;
		shakeyCam->addTrauma(maintenanceTrauma);
	}
}


void ChargeAbility::render()
{
	if (mState == AbilityState::Selected)
	{
		Quad2D<float> travelQuad = Camera::Get()->toCameraCoords(mSelectionQuad);
		renderQuadOutline(travelQuad, mSelectHighlight);
	}
	else if (mState == AbilityState::Running)
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

	AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::Play, "HitEnemy", mCaster));
}


void ChargeAbility::exit()
{
	mTimer.stop();
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

	Attribute* attribute = mCaster->getAttribute(AttributeType::Health);
	Health* health = static_cast<Health*>(attribute);
	health->setInvulnerablity(isCharging);

	if (isCharging)
	{
		mTimer.start();
		mAnimator.start();
		AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::Play, "ChargeAbility", mCaster));
	}
	else
	{
		mTimer.stop();
		mAnimator.stop();
		AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::FadeOut, "ChargeAbility", mCaster, 500));
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


void ChargeAbility::updateQuad()
{
	mRect.SetCenter(mCaster->position());

	RectF scaledRect = mRect;
	scaledRect.SetSize(mRect.Size() * 0.6f);
	scaledRect.SetCenter(mRect.Center());

	mQuad = Quad2D<float>(scaledRect);
	mQuad.rotate(rotation(), scaledRect.Center());
}


void ChargeAbility::updateSelectionQuad()
{
	mRect.SetCenter(mCaster->position());

	RectF rect = mRect;
	float travelDistance = mProperties.at(PropertyType::Velocity) * mProperties.at(PropertyType::Time);
	VectorF size(travelDistance, rect.Height() * 0.6f);
	rect.SetSize(size);
	rect.SetLeftCenter(mCaster->position());

	mSelectionQuad = Quad2D<float>(rect);
	mSelectionQuad.rotate(rotation(), mRect.Center());
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