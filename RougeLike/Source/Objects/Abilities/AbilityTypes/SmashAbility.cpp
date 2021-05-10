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

#include "Audio/AudioManager.h"


SmashAbility::SmashAbility(Texture* hammerTexture, RectF hammerRect)
{
	mHammer.texture = hammerTexture;
	float w = hammerRect.Width();
	float h = hammerRect.Height();
	mHammer.quad = Quad2D<float>(hammerRect);
	mHammer.collider.init(&mHammer.quad);

	mHammer.quad.rotate(45, mHammer.quad.center());

	float wid = mHammer.quad.width();
	float hig = mHammer.quad.height();


	int a = 4;

}


void SmashAbility::handleInput(const InputManager* input)
{
	VectorF cameraPosition = Camera::Get()->rect().TopLeft();
	VectorF cursorPos = input->cursorPosition() + cameraPosition;

	mTargetPosition = cursorPos;
	mHammer.direction = (mTargetPosition - mCaster->position()).normalise();

	updateSelectionQuad();
}


void SmashAbility::init()
{
	Texture* rangeCircle = TextureManager::Get()->getTexture("RangeCircle", FileManager::Image_UI);
	mRangeCircle = rangeCircle;

	mCollider = new QuadCollider(&mHammer.quad);
	mSelectionCollider.init(&mSelectionQuad);

	mWallCollisions.setCollider(&mHammer.collider);
}


void SmashAbility::activate()
{
	// Splash animation
	mAnimator.selectAnimation(Animation::Active);
	mRect.SetCenter(mTargetPosition);

	// Hammer logic
	mHammer.quad.setCenter(mCaster->position());

	// set quad
	//mHammer.quad = Quad2D<float>(mHammer.rect);
	mHammer.quad.rotate(getRotation(mHammer.direction) + 90.0f, mHammer.quad.center());

	// Set displacement source from the throw position
	mProperties.addXYPosition(mCaster->position());

	mActivateCollisions = true;

	AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::Play, "HammerThrow", mCaster));
}


void SmashAbility::fastUpdate(float dt)
{
	if (!mReachedTarget)
	{
		VectorF velocity = mHammer.direction * mProperties[PropertyType::Velocity] * dt;
		//mHammer.rect = mHammer.rect.Translate(velocity);
		mHammer.quad.translate(velocity);
	}
}


void SmashAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	VectorF currentDirection = (mTargetPosition - mHammer.quad.center()).normalise();
	bool hasPassedTarget = !((currentDirection.x > 0 == mHammer.direction.x > 0) && (currentDirection.y > 0 == mHammer.direction.y > 0));

	VectorF velocity = mHammer.direction * mProperties[PropertyType::Velocity] * dt;
	// increase the velocity here so we can look ahead a little
	velocity = mWallCollisions.allowedVelocity(mCaster->currentMap(), velocity * 5.0f, dt);
	// BUG: if the framerate is high dt can be 0?? this will end it early
	bool hitWall = (velocity.x == 0.0f || velocity.y == 0.0f) && dt != 0.0f;

	if (!mReachedTarget && (hasPassedTarget || hitWall))
	{
		explode();
	}

	// Completed one animation loop 
	if (mAnimator.loops() > 0)
	{
		mCompleted = true;
		mAnimator.stop(); // BaseExit calls animator.reset(), so we dont need to stop?
	}
}


void SmashAbility::explode()
{
	mReachedTarget = true;
	mTargetPosition = mHammer.quad.center();
	mRect.SetCenter(mTargetPosition);

	mAnimator.start();
	Camera::Get()->getShake()->addTrauma(140);

	// Final explosion uses mRect not the hammer rect for collision detection
	//mHammer.quad = Quad2D<float>(mRect);
	mHitList.clear();

	AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::Play, "Explosion", mCaster));
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

		// while hammer is flying
		if (!mReachedTarget)
		{
			Quad2D<float> hammerQuad = Camera::Get()->toCameraCoords(mHammer.quad); // NOTE: is this not working?
			//VectorF aboutPoint = hammerRect.Size() / 2.0f;

			RectF hammerRect;
			hammerRect.SetSize(hammerQuad.height(), hammerQuad.width());
			hammerRect.SetCenter(hammerQuad.center());

			mHammer.texture->render(hammerRect, getRotation(mHammer.direction) + 90.0f, hammerQuad.center());
		}
		else
		{
			RectF renderRect = mRect;
			renderRect.SetSize(mRect.Size() * 1.5f);
			renderRect.SetCenter(mRect.Center());
			renderRect = Camera::Get()->toCameraCoords(renderRect);

			mAnimator.render(renderRect);
		}

		printf("Hammer size %f, %f\n", mHammer.quad.width(), mHammer.quad.height());
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
	{
		applyHammerEffects(actor, effectPool);
		AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::Play, "HitEnemy", mCaster));
	}
	else
	{
		applyExplosionEffects(actor, effectPool);
	}
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
	//mHammer.quad.setCenter(mCaster->position());

	float travelDistance = distance(mCaster->position(), mTargetPosition);
	VectorF size(travelDistance, mHammer.quad.height());

	RectF rect;
	rect.SetSize(size);
	rect.SetLeftCenter(mCaster->position());

	mSelectionQuad = Quad2D<float>(rect);
	mSelectionQuad.rotate(getRotation(mHammer.direction), mCaster->position());
}