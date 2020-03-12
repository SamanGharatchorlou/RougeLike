#pragma once
#include "Characters/Attributes/Damage.h"

class Collider
{
public:
	Collider();
	Collider(RectF* rect, VectorF relativeColliderSize = VectorF(1.0f, 1.0f));
	virtual ~Collider() { }

	void init(RectF* objectRect, VectorF colliderScale = VectorF(1.0f, 1.0f));
	void reset();

	bool doesIntersect(Collider* collider) const;

	void hasCollidedWith(Collider* collider);

	inline void didCollide(bool collision) { mHasCollided = collision; }
	inline bool hasCollided() const { return mHasCollided; }

	inline void setDidHit(bool collisionStatus) { mDidHit = collisionStatus; }
	inline bool didHit() const { return mDidHit; }

	inline void setGotHit(bool collisionStatus) { mGotHit = collisionStatus; }
	inline bool gotHit() const { return mGotHit; }

	const RectF getRect() const { return *mRect; }
	RectF getRectBase() const;

	const Collider* getOtherCollider() const { return mOtherCollider; }
	const Damage getOtherColliderDamage() const;

	void setActive(bool isActive) { mIsActive = isActive; }
	bool isActive() const { return mIsActive; }

protected:
	const RectF* mRect;
	VectorF mColliderScale;

	Collider* mOtherCollider;

	bool mHasCollided;

	bool mIsActive;

	// TODO: replace has collider with these
	bool mDidHit;
	bool mGotHit;
};