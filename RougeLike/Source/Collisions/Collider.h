#pragma once
#include "Characters/Attributes/Damage.h"

class Collider
{
public:
	Collider();
	Collider(Damage damage);
	Collider(RectF* rect, VectorF relativeColliderSize = VectorF(1.0f, 1.0f));

	void init(RectF* objectRect, VectorF colliderScale = VectorF(1.0f, 1.0f));

	bool doesIntersect(Collider* collider) const;

	void hasCollidedWith(Collider* collider);
	inline void didCollide(bool collision) { mHasCollided = collision; }
	inline bool hasCollided() const { return mHasCollided; }

	const RectF getRect() const { return *mRect; }
	RectF getRectBase() const;

	void setDamage(Damage damage) { mDamage = damage; }
	const Damage getDamage() const { return mDamage; }

	const Collider* getOtherCollider() const { return mOtherCollider; }
	const Damage getOtherColliderDamage() const { return mOtherCollider->getDamage(); }

	void setActive(bool isActive) { mIsActive = isActive; }
	bool isActive() const { return mIsActive; }

private:
	const RectF* mRect;
	VectorF mColliderScale;

	Collider* mOtherCollider;

	Damage mDamage;

	bool mHasCollided;

	bool mIsActive;
};