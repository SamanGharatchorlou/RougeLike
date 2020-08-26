#include "pch.h"
#include "Collider.h"

#if TRACK_COLLISIONS
#include "Debug/DebugDraw.h"
#endif


Collider::Collider() :
	mRect(nullptr),
	mColliderScale(VectorF(1.0f, 1.0f)),
	mOtherCollider(nullptr),
	mDidHit(false),
	mGotHit(false)
{ }


Collider::Collider(RectF* rect, VectorF colliderScale) : 
	mRect(rect),
	mColliderScale(colliderScale),
	mOtherCollider(nullptr),
	mDidHit(false),
	mGotHit(false)
{ }


void Collider::init(RectF* objectRect, VectorF colliderScale) 
{ 
	mRect = objectRect; 
	mColliderScale = colliderScale;
}


void Collider::reset()
{
	mGotHit = false;
	mDidHit = false;
	mOtherCollider = nullptr;
}


// Build collider out of a fraction of the container rect
RectF Collider::scaledRect() const
{
	RectF colliderRect = *mRect;
	colliderRect.SetSize(mRect->Size() * mColliderScale);
	colliderRect.SetCenter(mRect->Center());

	return colliderRect;
}


bool Collider::doesIntersect(Collider* collider) const
{
	RectF thisRect = scaledRect();
	RectF thatRect = collider->scaledRect();

	bool xOverlaps = thisRect.LeftPoint() < thatRect.RightPoint() && thisRect.RightPoint() > thatRect.LeftPoint();
	bool yOverlaps = thisRect.TopPoint() < thatRect.BotPoint() && thisRect.BotPoint() > thatRect.TopPoint();

	return xOverlaps && yOverlaps;
}


void Collider::hasCollidedWith(Collider* collider)
{
	mOtherCollider = collider;
}


bool Collider::contains(VectorF position)
{
	bool xOverlaps = mRect->LeftPoint() < position.x && mRect->RightPoint() > position.x;
	bool yOverlaps = mRect->TopPoint() < position.y && mRect->BotPoint() > position.y;

	return xOverlaps && yOverlaps;
}


#if TRACK_COLLISIONS
void Collider::renderCollider()
{
	if (didHit())
	{
		debugDrawRect(scaledRect(), RenderColour::Blue);
	}
	else if(gotHit())
	{
		debugDrawRect(scaledRect(), RenderColour::Red);
	}
	else
	{
		debugDrawRect(scaledRect(), RenderColour::LightGrey);
	}
}
#endif


bool test1DOverlap(float minA, float maxA, float minB, float maxB)
{
	return maxA > minB && minA < maxB;
}