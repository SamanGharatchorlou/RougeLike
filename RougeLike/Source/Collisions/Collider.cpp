#include "pch.h"
#include "Collider.h"


Collider::Collider() :
	mRect(nullptr),
	mColliderScale(VectorF(1.0f, 1.0f)),
	mOtherCollider(nullptr),
	mHasCollided(false),
	mIsActive(true)
{ }


Collider::Collider(Damage damage) :
	mDamage(damage),
	mRect(nullptr),
	mColliderScale(VectorF(1.0f, 1.0f)),
	mOtherCollider(nullptr),
	mHasCollided(false),
	mIsActive(true)
{ }


Collider::Collider(RectF* rect, VectorF colliderScale) : 
	mRect(rect),
	mColliderScale(colliderScale),
	mOtherCollider(nullptr),
	mHasCollided(false),
	mIsActive(true)
{ }


void Collider::init(RectF* objectRect, VectorF colliderScale) 
{ 
	mRect = objectRect; 
	mColliderScale = colliderScale;
}

// convert the player rect to just return the base of the player i.e. their effective feet area
RectF Collider::getRectBase() const 
{
	RectF colliderRect = *mRect;

	float rectWidth = mRect->Width();
	float colliderWidth = rectWidth * mColliderScale.x;

	float rectHeight = mRect->Height();
	float colliderHeight = rectHeight * mColliderScale.y;

	float xPosition = mRect->LeftPoint() + ((rectWidth - colliderWidth) / 2.0f);
	float yPosition = mRect->BotPoint() - colliderHeight;

	return RectF(xPosition, yPosition, xPosition + colliderWidth, yPosition + colliderHeight);
}


bool Collider::doesIntersect(Collider* collider) const
{
	RectF thisRect = getRectBase();
	RectF thatRect = collider->getRectBase();

	bool xOverlaps = thisRect.LeftPoint() < thatRect.RightPoint() && thisRect.RightPoint() > thatRect.LeftPoint();
	bool yOverlaps = thisRect.TopPoint() < thatRect.BotPoint() && thisRect.BotPoint() > thatRect.TopPoint();

	return xOverlaps && yOverlaps;
}


bool Collider::doesIntersect(RectF rect) const
{
	RectF thisRect = getRectBase();
	RectF thatRect = rect;

	bool xOverlaps = thisRect.LeftPoint() < thatRect.RightPoint() && thisRect.RightPoint() > thatRect.LeftPoint();
	bool yOverlaps = thisRect.TopPoint() < thatRect.BotPoint() && thisRect.BotPoint() > thatRect.TopPoint();

	return xOverlaps && yOverlaps;
}


void Collider::hasCollidedWith(Collider* collider)
{
	mOtherCollider = collider;
}
