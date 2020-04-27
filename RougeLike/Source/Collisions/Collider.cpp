#include "pch.h"
#include "Collider.h"

#include "DamageCollider.h"


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


// convert the player rect to just return the base of the player i.e. their effective feet area
RectF Collider::scaledRect() const
{
	RectF colliderRect = *mRect;

	float colliderWidth = mRect->Width() * mColliderScale.x;
	float colliderHeight = mRect->Height() * mColliderScale.y;

	float xPosition = mRect->LeftPoint() + ((mRect->Width() - colliderWidth) / 2.0f);
	float yPosition = mRect->BotPoint() - colliderHeight;

	return RectF(xPosition, yPosition, xPosition + colliderWidth, yPosition + colliderHeight);
}


bool Collider::doesIntersect(Collider* collider) const
{
	RectF thisRect = scaledRect();
	RectF thatRect = collider->rect();

	bool xOverlaps = thisRect.LeftPoint() < thatRect.RightPoint() && thisRect.RightPoint() > thatRect.LeftPoint();
	bool yOverlaps = thisRect.TopPoint() < thatRect.BotPoint() && thisRect.BotPoint() > thatRect.TopPoint();

	return xOverlaps && yOverlaps;
}


void Collider::hasCollidedWith(Collider* collider)
{
	mOtherCollider = collider;
}
