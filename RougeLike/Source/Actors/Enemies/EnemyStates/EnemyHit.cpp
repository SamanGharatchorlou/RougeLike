#include "pch.h"
#include "EnemyHit.h"

#include "Actors/Enemies/Enemy.h"

#include "Game/Camera/Camera.h"
#include "Graphics/Texture.h"


void EnemyHit::init()
{
	mEnemy->animator().selectAnimation(Action::Hurt);

	Texture* texture = mEnemy->animator().texture();
	texture->modifyAlpha(-100);
}


void EnemyHit::slowUpdate(float dt)
{
	mEnemy->resolveCollisions();

	if (mEnemy->animator().loops() > 0)
	{
		mEnemy->popState();
	}
}


void EnemyHit::render()
{
	RectF rect = Camera::Get()->toCameraCoords(mEnemy->rect());

#if DRAW_ENEMY_RECT
	debugDrawRect(mEnemy->rect(), RenderColour(RenderColour::Red));
#else
	mEnemy->animator().render(rect, mEnemy->physics()->flip());
#endif
}


void EnemyHit::exit()
{
	Texture* texture = mEnemy->animator().texture();
	texture->setAlpha(alphaMax);
}


void EnemyHit::resume()
{
	mEnemy->popState();
}