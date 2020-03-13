#include "pch.h"
#include "EnemyHit.h"

#include "Game/GameData.h"
#include "Characters/Enemies/Enemy.h"
#include "Characters/Player/Player.h"

#include "Graphics/Texture.h"


EnemyHit::EnemyHit(Enemy* enemy) : mEnemy(enemy)
{
	mEnemy->getCollider()->reset();
}


void EnemyHit::init()
{
	timer.restart();

	mEnemy->getAnimator()->selectAnimation("Hit");

	Texture* texture = mEnemy->getAnimator()->getSpriteTexture();
	texture->modifyAlpha(-100);

	// Player to Enemy unit direction vector
	VectorF enemyPosition = mEnemy->getMovement().getPostion();
	VectorF source = mEnemy->targetRect().Center();
	VectorF hitDirection = (enemyPosition - source).normalise();

	mEnemy->getMovement().setDirection(hitDirection);

	const Damage damage = mEnemy->getCollider()->getOtherColliderDamage();

	Health newHealth = mEnemy->propertyBag().pHealth.get() - damage;

	mEnemy->propertyBag().pHealth.set(newHealth);
	printf("hit state\n");
}


void EnemyHit::slowUpdate(float dt)
{
	if (timer.getSeconds() > mEnemy->propertyBag().pHurtTime.get())
	{
		mEnemy->replaceState(EnemyState::Run);
	}

	if (mEnemy->propertyBag().pHealth.get().isDead())
	{
		mEnemy->replaceState(EnemyState::Dead);
	}
}


void EnemyHit::fastUpdate(float dt)
{
	mEnemy->getMovement().fastUpdate(dt);
}


void EnemyHit::exit()
{
	Texture* texture = mEnemy->getAnimator()->getSpriteTexture();
	texture->setAlpha(alphaMax);

	mEnemy->getMovement().setDirection(VectorF());
}