#pragma once

#include "Animations/Animator.h"
#include "Collisions/Colliders/EffectCollider.h"

class MagicWeaponData;

class MagicProjectile
{
public:
	MagicProjectile(const MagicWeaponData* data);

	void fire(VectorF position, VectorF direction);

	void move(float dt);

	void render() const;

private:
	Animator mAnimator;

	EffectCollider mCollider;
	RectF mRect;

	float mSpeed;
	VectorF mDirection;
};