#include "pch.h"
#include "MagicProjectile.h"

#include "Weapons/WeaponData.h"

MagicProjectile::MagicProjectile(const MagicWeaponData* data)
{
	mAnimator = data->animator;
	mSpeed = data->effectData.at(PropertyType::Velocity);

	VectorF size(data->maxDimention, data->maxDimention);
	mRect.SetSize(size);

	mCollider.init(&mRect);
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
	mAnimator.render(mRect);
}