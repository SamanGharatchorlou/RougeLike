#include "pch.h"
#include "EnemyDead.h"

#include "Characters/Enemies/Enemy.h"

#include "Game/Camera.h"

// TODO: this should come from enemy?
#include "Graphics/Texture.h"



EnemyDead::EnemyDead(Enemy* enemy) : EnemyState(enemy), mAlpha(alphaMax) { }


void EnemyDead::init()
{
	mEnemy->getAnimator()->selectAnimation("Dead");

	// Let everyone know he has died
	int score = mEnemy->propertyBag().pScore.get();
	int exp = mEnemy->propertyBag().pExp.get();
	EnemyDeadEvent* dataPtr = new EnemyDeadEvent(score, exp);

	mEnemy->pushEvent(EventPacket(Event::EnemyDead, dataPtr));
}


void EnemyDead::slowUpdate(float dt)
{
	mAlpha -= (Uint8)(200 * dt);

	// Remove enemy from play
	if (mAlpha <= 10)
	{
		mEnemy->addState(EnemyState::None);
	}
}


void EnemyDead::render()
{
	// Flip sprite
	if (mEnemy->flip() == SDL_FLIP_NONE && mEnemy->getMovement().getDirection().x < 0)
	{
		mEnemy->setFlip(SDL_FLIP_HORIZONTAL);
	}
	else if (mEnemy->flip() == SDL_FLIP_HORIZONTAL && mEnemy->getMovement().getDirection().x >= 0)
	{
		mEnemy->setFlip(SDL_FLIP_NONE);
	}

	// render in camera coords
	VectorF cameraPosition = Camera::Get()->toCameraCoords(mEnemy->getMovement().getPostion());
	RectF rect = RectF(cameraPosition, mEnemy->getRect().Size());

	mEnemy->getAnimator()->getSpriteTile()->render(rect, mEnemy->flip(), mAlpha);

#if DRAW_ENEMY_RECT
	debugDrawRect(mRect, RenderColour(RenderColour::RED));
#endif
}