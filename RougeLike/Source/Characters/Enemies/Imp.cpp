#include "pch.h"

#include "Imp.h"
#include "Game/GameData.h"


Imp::Imp(GameData* gameData, AIPathMap* map) : Enemy(gameData, map) { }


void Imp::init()
{
	initAnimations("Imp.xml");
	bag.readAttributes("Imp.xml");

	// Size
	VectorF size = mAnimator.getSpriteTile()->getRect().Size() * 1.5f;
	colliderRatio = VectorF(0.75f, 1.0f);
	mRect = RectF(VectorF(), size * colliderRatio);

	// Movement
	mMovement.setPosition(mRect.TopLeft());

	mCollider.init(&mRect);
	mCollider.setDamage(propertyBag().pDamage.get());

	// TODO: using int movement speed for float in mMovement class, fix me
	mMovement.init(&mCollider, (float)bag.pMovementSpeed.get());
}


RectF Imp::renderRect() const
{
	RectF rect = mRect;
	VectorF size = rect.Size() / colliderRatio;
	
	rect.SetSize(size * 1.4f);
	VectorF sizeDiff = rect.Size() - size;

	// Tweak position
	return rect.Translate((sizeDiff.x * -1) + 2.0f, (sizeDiff.y * -1) + 2.0f);;
}