#include "pch.h"
#include "MagicProjectile.h"
#include "Game/Camera/Camera.h"
#include "Weapons/WeaponData.h"

#if DEBUG_CHECK
#include "Debug/DebugDraw.h"
#endif


MagicProjectile::MagicProjectile() : mDeactivated(false), mSpeed(0.0f) { }


void MagicProjectile::init(const MagicWeaponData* data)
{
	mAnimator = data->animator;
	mAnimator.selectAnimation(Animation::Action::Active);
	mSpeed = data->effectData.at(PropertyType::Velocity);

	VectorF size(data->maxDimention, data->maxDimention);
	mRect.SetSize(size);

	mCollider.init(&mRect, data->projectileColliderScale);
}


void MagicProjectile::fire(VectorF position, VectorF direction)
{
	mDirection = direction;
	mRect.SetCenter(position);
	mAnimator.start();
}


void MagicProjectile::move(float dt)
{
	mRect = mRect.Translate(mDirection * mSpeed * dt);
}


void MagicProjectile::render() const
{
#if TRACK_COLLISIONS
	mCollider.renderCollider();
#else
#if DRAW_PLAYER_WEAPON_RECT
	debugDrawRect(mRect, RenderColour::Purple);
#endif
#endif

	RectF rect = Camera::Get()->toCameraCoords(mRect);

	VectorF aboutPoint = rect.Size() / 2.0f;


	mAnimator.render(rect, getRotation(mDirection), aboutPoint);
}

void MagicProjectile::reset()
{
	mAnimator.stop();
	mCollider.reset();
	mDirection.set(0.0f, 0.0f);
	mDeactivated = false;
}