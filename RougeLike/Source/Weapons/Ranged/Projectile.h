#pragma once

#include "Weapons/WeaponData.h"

#include "Collisions/DamageCollider.h"

class Texture;

class Projectile
{
public:
	Projectile(const RangedWeaponData& data);

	void fire(VectorF position, VectorF direction);

	void move(float dt);


private:
	Texture* mTexture;

	RectF mRect;

	DamageCollider mCollider;

	float mSpeed;
	VectorF mDirection;
};