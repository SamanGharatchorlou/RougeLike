#include "pch.h"

#include "Imp.h"
#include "Game/GameData.h"


Imp::Imp(GameData* gameData) : Enemy(gameData) { }


void Imp::init()
{
	initAnimations("Imp.xml");
	mBag.readAttributes("Imp.xml");

	// Size
	VectorF size = mAnimator.getSpriteTile()->getRect().Size() * 1.5f;
	colliderRatio = VectorF(0.75f, 1.0f);
	RectF rect = RectF(VectorF(), size * colliderRatio);

	mPhysics.setRect(RectF(VectorF(), size * colliderRatio));

	mCollider.init(&mPhysics.rectRef());
	mCollider.set(propertyBag().pDamage.get(), propertyBag().pKnockbackDistance.get());

	Enemy::init();
}


RectF Imp::renderRect() const
{
	RectF rect = mPhysics.rect();
	VectorF size = rect.Size() / colliderRatio;
	
	rect.SetSize(size * 1.4f);
	VectorF sizeDiff = rect.Size() - size;

	// Tweak position
	return rect.Translate((sizeDiff.x * -1) + 2.0f, (sizeDiff.y * -1) + 2.0f);;
}