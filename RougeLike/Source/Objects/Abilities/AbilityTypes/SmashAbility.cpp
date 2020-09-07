#include "pch.h"
#include "SmashAbility.h"

#include "Animations/Animator.h"
#include "Game/Camera/Camera.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"
#include "Input/InputManager.h"

#include "Actors/Actor.h"
#include "Objects/Pools/EffectPool.h"
#include "Collisions/Colliders/QuadCollider.h"


// TODO: fix the placement of this i.e. its position on the map, now that its large its hard to place
SmashAbility::SmashAbility(Texture* hammerTexture, RectF hammerRect) : mHammerTexture(hammerTexture), mHammerRect(hammerRect) { }


void SmashAbility::handleInput(const InputManager* input)
{
	VectorF cameraPosition = Camera::Get()->rect().TopLeft();
	VectorF cursorPos = input->cursorPosition() + cameraPosition;

	mTargetPosition = cursorPos;
	mHammerDirection = (mTargetPosition - mCaster->position()).normalise();

	updateSelectionQuad();
}


void SmashAbility::init()
{
	Texture* rangeCircle = TextureManager::Get()->getTexture("RangeCircle", FileManager::Image_UI);
	mRangeCircle = rangeCircle;

	mCollider = new QuadCollider(&mQuad);
	mSelectionCollider.init(&mSelectionQuad);

	mWallCollisions.setRect(&mHammerRect);
}


void SmashAbility::activate()
{
	// Splash animation
	mAnimator.selectAnimation(Action::Active);
	mRect.SetCenter(mTargetPosition);

	// Hammer logic
	mHammerRect.SetCenter(mCaster->position());

	// set quad
	mQuad = Quad2D<float>(mHammerRect);
	mQuad.rotate(getRotation(mHammerDirection), mHammerRect.Center());

	// Set displacement source from the throw position
	mProperties.addXYPosition(mCaster->position());

	mActivateCollisions = true;
}


void SmashAbility::fastUpdate(float dt)
{
	if (!mReachedTarget)
	{
		VectorF velocity = mHammerDirection * mProperties[PropertyType::Velocity] * dt;
		mHammerRect = mHammerRect.Translate(velocity);
		mQuad.translate(velocity);
	}
}


void SmashAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	// has the direction to the target changed i.e. passed it?
	VectorF currentDirection = (mTargetPosition - mHammerRect.Center()).normalise();
	bool hasPassedTarget = !((currentDirection.x > 0 == mHammerDirection.x > 0) && (currentDirection.y > 0 == mHammerDirection.y > 0));

	VectorF velocity = mHammerDirection * mProperties[PropertyType::Velocity] * dt;
	// increase the velocity here so we can look ahead a little
	velocity = mWallCollisions.allowedVelocity(mCaster->currentMap(), velocity * 5.0f, dt);
	bool hitWall = velocity.x == 0.0f || velocity.y == 0.0f;


	// Begin explosion
	if (!mReachedTarget && (hasPassedTarget || hitWall))
	{
		mReachedTarget = true;
		mTargetPosition = mHammerRect.Center();
		mRect.SetCenter(mTargetPosition);

		mAnimator.start();

		// Final explosion uses mRect not the hammer rect for collision detection
		mQuad = Quad2D<float>(mRect);
		mHitList.clear();
	}

	// Completed one animation loop 
	if (mAnimator.loops() > 0)
	{
		mCompleted = true;
		mAnimator.stop();
	}
}


void SmashAbility::render()
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
		debugDrawRect(mHammerRect, RenderColour::Yellow);
		debugDrawLine(mHammerRect.Center(), mTargetPosition, RenderColour::Black);
#endif
#if TRACK_COLLISIONS
		mCollider->renderCollider();
#endif

		RectF renderRect = mRect; Camera::Get()->toCameraCoords(mRect);
		renderRect.SetSize(mRect.Size() * 1.5f);
		renderRect.SetCenter(mRect.Center());
		renderRect = Camera::Get()->toCameraCoords(renderRect);

		mAnimator.render(renderRect);

		// while hammer is flying
		if (!mReachedTarget)
		{
			RectF hammerRect = Camera::Get()->toCameraCoords(mHammerRect);
			VectorF aboutPoint = hammerRect.Size() / 2.0f;
			mHammerTexture->render(hammerRect, getRotation(mHammerDirection), aboutPoint);
		}
	}
}

void SmashAbility::exit()
{
	mReachedTarget = false;
}


// --- Private Functions --- //

void SmashAbility::applyEffects(Actor* actor, EffectPool* effectPool)
{
	if (!mReachedTarget)
		applyHammerEffects(actor, effectPool);
	else
		applyExplosionEffects(actor, effectPool);
}


void SmashAbility::applyHammerEffects(Actor* actor, EffectPool* effectPool)
{
	mProperties.addXYPosition(mCaster->position());

	applyEffect(EffectType::Damage, actor, effectPool);

	float distance = mProperties[PropertyType::KnockbackDistance];
	mProperties[PropertyType::KnockbackDistance] = distance / 3.0f;

	applyEffect(EffectType::Displacement, actor, effectPool);

	mProperties[PropertyType::KnockbackDistance] = distance;
}


void SmashAbility::applyExplosionEffects(Actor* actor, EffectPool* effectPool)
{
	mProperties.addXYPosition(mCaster->position());

	applyEffect(EffectType::Damage, actor, effectPool);
	applyEffect(EffectType::Displacement, actor, effectPool);
	applyEffect(EffectType::Stun, actor, effectPool);
}


void SmashAbility::updateSelectionQuad()
{
	mHammerRect.SetCenter(mCaster->position());
	RectF rect = mHammerRect;

	float travelDistance = distance(mHammerRect.Center(), mTargetPosition);
	VectorF size(travelDistance, rect.Height());
	rect.SetSize(size);
	rect.SetLeftCenter(mHammerRect.Center());

	mSelectionQuad = Quad2D<float>(rect);
	mSelectionQuad.rotate(getRotation(mHammerDirection) - 90, mHammerRect.Center());
}