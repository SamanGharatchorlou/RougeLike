#include "pch.h"

#include "Imp.h"


Imp::Imp(GameData* gameData) : Enemy(gameData) { }


RectF Imp::renderRect() const
{
	RectF rect = mPhysics.rect();
	VectorF size = rect.Size() / mColliderRatio;
	
	rect.SetSize(size * 1.4f);
	VectorF sizeDiff = rect.Size() - size;

	// Tweak position
	return rect.Translate((sizeDiff.x * -1) + 2.0f, (sizeDiff.y * -1) + 2.0f);
}