#include "pch.h"
#include "EnemyHit.h"

#include "Objects/Actors/Enemies/Enemy.h"

#include "Game/Camera.h"
#include "Graphics/Texture.h"

#include "Collisions/DamageCollider.h"

#include "Map/Map.h"


EnemyHit::EnemyHit(Enemy* enemy) : EnemyState(enemy)  { }


void EnemyHit::init()
{
	printf("hit\n");
	decayTimer.restart();

	mEnemy->animator().selectAnimation("Hit");

	Texture* texture = mEnemy->animator().getSpriteTexture();
	texture->modifyAlpha(-100);

	attackTargetPosition = mEnemy->attackTargetRect()->Center();
	mEnemy->physics()->facePoint(attackTargetPosition);
}


void EnemyHit::slowUpdate(float dt)
{
	if (decayTimer.getSeconds() > mEnemy->getPropertyValue("HurtTime"))
	{
		Health* health = static_cast<Health*>(mEnemy->getProperty("Health"));

		if (health->isDead())
			mEnemy->replaceState(EnemyState::Dead);
		else
			mEnemy->replaceState(EnemyState::Run);
	}
}


void EnemyHit::render()
{
	RectF rect = mEnemy->renderRect();
	rect = Camera::Get()->toCameraCoords(rect);

#if DRAW_ENEMY_RECT
	debugDrawRect(mEnemy->rect(), RenderColour(RenderColour::Red));
#else
	mEnemy->animator().getSpriteTile()->render(rect, mEnemy->physics()->flip());
#endif
}


void EnemyHit::exit()
{
	Texture* texture = mEnemy->animator().getSpriteTexture();
	texture->setAlpha(alphaMax);

	mEnemy->physics()->facePoint(mEnemy->attackTargetRect()->Center());
}