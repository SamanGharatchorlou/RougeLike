#include "pch.h"
#include "Imp.h"

Imp::Imp(GameData* gameData) : Enemy(gameData) { }

void Imp::init()
{
	Enemy::init("Imp");
}

RectF Imp::renderRect() const
{
	RectF rect = mPhysics.rect();
	VectorF size = rect.Size() / mColliderRatio;
	
	float scale = 1.7f;

	rect.SetSize(size * scale);
	VectorF sizeDiff = rect.Size() - size;

	// Tweak position
	return rect.Translate((sizeDiff.x * -1) + 2.0f, (sizeDiff.y * -1) + 2.0f);
}