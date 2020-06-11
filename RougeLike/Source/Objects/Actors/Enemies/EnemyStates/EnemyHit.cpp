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
	//printf("hit\n");
	mEnemy->animator().selectAnimation(Action::Hurt);

	Texture* texture = mEnemy->animator().texture();
	texture->modifyAlpha(-100);

	mEnemy->physics()->facePoint(mEnemy->target()->position());

	Health* health = static_cast<Health*>(mEnemy->getProperty("Health"));

	if (health->isDead())
		mEnemy->addState(EnemyState::Dead);
}


void EnemyHit::slowUpdate(float dt)
{
	mEnemy->resolveCollisions();

	if (mEnemy->animator().loops() > 0)
	{
		Health* health = static_cast<Health*>(mEnemy->getProperty("Health"));

		if (health->isDead())
			mEnemy->addState(EnemyState::Dead);
		else
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

	mEnemy->physics()->facePoint(mEnemy->target()->position());
}


void EnemyHit::resume()
{
	mEnemy->popState();
}