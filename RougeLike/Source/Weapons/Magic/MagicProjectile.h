#pragma once

#include "Animations/Animator.h"
#include "Collisions/Colliders/Collider.h"

class MagicWeaponData;

class MagicProjectile
{
	friend class MagicWeapon;

public:
	MagicProjectile();

	void init(const MagicWeaponData* data);

	void fire(VectorF position, VectorF direction);

	void move(float dt);

	void render() const;

	void reset();

private:
	Animator mAnimator;

	Collider mCollider;
	RectF mRect;

	float mSpeed;
	VectorF mDirection;

	bool mDeactivated;
};