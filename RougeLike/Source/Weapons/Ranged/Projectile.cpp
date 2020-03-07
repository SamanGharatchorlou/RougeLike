#include "pch.h"
#include "Projectile.h"

#include "Graphics/Texture.h"


Projectile::Projectile(const RangedWeaponData* data) : mActive(true)
{
	mTexture = data->projectileTexture;
	mRect.SetSize(data->projectileSize);
	mCollider.init(&mRect);
	mSpeed = data->travelSpeed;
}


void Projectile::init(const RangedWeaponData* data)
{
	mTexture = data->projectileTexture;
	mRect.SetSize(data->projectileSize);
	mCollider.init(&mRect);
	mSpeed = data->travelSpeed;
	mActive = true;
}


void Projectile::reset()
{
	mDirection.zero();
	mCollider.reset();
	mActive = true;
}


void Projectile::fire(VectorF position, VectorF direction)
{
	mRect.SetCenter(position);
	mDirection = direction;
	mActive = true;
}


void Projectile::move(float dt)
{
	if(mActive)
		mRect = mRect.Translate(mDirection.normalise() * mSpeed * dt);
}

void Projectile::render()
{
	if(mActive)
		mTexture->render(mRect);
}