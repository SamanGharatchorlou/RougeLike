#pragma once

class Actor;

class Collider
{
public:
	Collider();
	Collider(RectF* rect, VectorF relativeColliderSize = VectorF(1.0f, 1.0f));
	virtual ~Collider() { }

	void init(RectF* objectRect, VectorF colliderScale = VectorF(1.0f, 1.0f));
	void reset();

	virtual bool doesIntersect(Collider* collider) const;
	void hasCollidedWith(Collider* collider);

	inline void setDidHit(bool didHit) { mDidHit = didHit; }
	inline bool didHit() const { return mDidHit; }

	inline void setGotHit(bool gotHit) { mGotHit = gotHit; }
	inline bool gotHit() const { return mGotHit; }

	bool contains(VectorF position);

	const RectF rect() const { return *mRect; }
	virtual RectF scaledRect() const;

	const Collider* getOtherCollider() const { return mOtherCollider; }
	Collider* getOtherCollider() { return mOtherCollider; }

	virtual float xMin() const;
	virtual float xMax() const;

#if TRACK_COLLISIONS
	virtual void renderCollider();	
#endif


protected:
	const RectF* mRect;
	VectorF mColliderScale;

	Collider* mOtherCollider;

	bool mDidHit;
	bool mGotHit;
};


bool test1DOverlap(float minA, float maxA, float minB, float maxB);

std::vector<int> broadPhaseIndexes(const Collider* collider, const std::vector<Actor*>* targets);