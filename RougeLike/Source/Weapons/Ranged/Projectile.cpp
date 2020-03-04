#include "pch.h"
#include "Projectile.h"


Projectile::Projectile(const RangedWeaponData& data)
{
	mTexture = data.projectileTexture;
	mRect.SetSize(data.projectileSize);
	mCollider.init(&mRect);
	mSpeed = data.travelSpeed;
}


void Projectile::fire(VectorF position, VectorF direction)
{
	mRect.SetCenter(position);
	mDirection = direction;
}


void Projectile::move(float dt)
{
	mRect.Translate(mDirection * mSpeed * dt);
}