#include "pch.h"
#include "Projectile.h"

#include "Graphics/Texture.h"


Projectile::Projectile(const RangedWeaponData* data)
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
}


void Projectile::reset()
{
	mDirection.zero();
	mCollider.reset();
}


void Projectile::fire(VectorF position, VectorF direction)
{
	mRect.SetCenter(position);
	mDirection = direction;
}


void Projectile::move(float dt)
{
	mRect = mRect.Translate(mDirection.normalise() * mSpeed * dt);
}

void Projectile::render(RectF rect) const
{
	mTexture->render(rect);
}