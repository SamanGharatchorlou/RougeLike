#include "pch.h"
#include "EnemyHit.h"

#include "Game/GameData.h"
#include "Characters/Enemies/Enemy.h"
#include "Characters/Player/Player.h"

#include "Game/Camera.h"
#include "Graphics/Texture.h"


EnemyHit::EnemyHit(Enemy* enemy) : EnemyState(enemy)
{
	mEnemy->getCollider()->reset();
}


void EnemyHit::init()
{
	timer.restart();

	mEnemy->getAnimator()->selectAnimation("Hit");

	Texture* texture = mEnemy->getAnimator()->getSpriteTexture();
	texture->modifyAlpha(-100);

	//mEnemy->getMovement().setSpeed(150);

	// Player to Enemy unit direction vector
	//VectorF enemyPosition = mEnemy->getMovement().getPostion();
	//VectorF source = mEnemy->targetPosition();
	//VectorF hitDirection = (enemyPosition - source).normalise();
	//mEnemy->getMovement().setDirection(hitDirection);

	// Set hp
	const Damage damage = mEnemy->getCollider()->getOtherColliderDamage();
	Health newHealth = mEnemy->propertyBag().pHealth.get() - damage;
	mEnemy->propertyBag().pHealth.set(newHealth);
}


void EnemyHit::fastUpdate(float dt)
{
	//mEnemy->move(dt); // TODO: replace this with fast update?
}


void EnemyHit::slowUpdate(float dt)
{
	if (timer.getSeconds() > mEnemy->propertyBag().pHurtTime.get())
	{
		if (mEnemy->propertyBag().pHealth.get().isDead())
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
	mEnemy->getAnimator()->getSpriteTile()->render(rect, mEnemy->flip());
#endif
}


void EnemyHit::exit()
{
	Texture* texture = mEnemy->getAnimator()->getSpriteTexture();
	texture->setAlpha(alphaMax);

	//mEnemy->getMovement().setDirection(VectorF());
}