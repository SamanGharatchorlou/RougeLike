#pragma once


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

	inline void setDidHit(bool collisionStatus) { mDidHit = collisionStatus; }
	inline bool didHit() const { return mDidHit; }

	inline void setGotHit(bool collisionStatus) { mGotHit = collisionStatus; }
	inline bool gotHit() const { return mGotHit; }

	bool contains(VectorF position);

	const RectF rect() const { return *mRect; }
	RectF scaledRect() const;

	const Collider* getOtherCollider() const { return mOtherCollider; }

#if _DEBUG
	void setName(const std::string& name) { mName = name; }
	std::string name() const { return mName; }
#endif

protected:
	const RectF* mRect;
	VectorF mColliderScale;

	Collider* mOtherCollider;

	bool mDidHit;
	bool mGotHit;

#if _DEBUG
	std::string mName;
#endif
};