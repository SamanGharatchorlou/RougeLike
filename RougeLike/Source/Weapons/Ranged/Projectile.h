#pragma once

#include "Weapons/WeaponData.h"
#include "Collisions/DamageCollider.h"

class Texture;

class Projectile
{
public:
	Projectile() { }
	Projectile(const RangedWeaponData* data);

	void init(const RangedWeaponData* data);
	void reset(); // TODO: do i need this?

	void fire(VectorF position, VectorF direction);

	void move(float dt);

	void render();

	bool hasCollided() const { return mCollider.hasCollided(); }


private:
	Texture* mTexture;

	RectF mRect;

	DamageCollider mCollider;

	float mSpeed;
	VectorF mDirection;
};