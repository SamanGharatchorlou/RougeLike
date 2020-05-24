#include "pch.h"
#include "Angel.h"

Angel::Angel(GameData* gameData) : Enemy(gameData) { }

void Angel::init()
{
	Enemy::init("Angel");
}

RectF Angel::renderRect() const
{
	RectF rect = mPhysics.rect();
	VectorF size = rect.Size() / mColliderRatio;

	rect.SetSize(size * 1.2f);
	VectorF sizeDiff = rect.Size() - size;

	// Tweak position
	return rect.Translate((sizeDiff.x * -1) + 2.0f, (sizeDiff.y * -1) + 2.0f);
}