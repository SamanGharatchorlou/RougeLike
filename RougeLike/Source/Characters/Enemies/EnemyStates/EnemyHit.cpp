#include "pch.h"
#include "EnemyHit.h"

#include "Game/GameData.h"
#include "Characters/Enemies/Enemy.h"
#include "Characters/Player/Player.h"

#include "Graphics/Texture.h"


void EnemyHit::init()
{
	timer.restart();

	mEnemy->getAnimator()->selectAnimation("Hit");

	Texture* texture = mEnemy->getAnimator()->getSpriteTexture();
	texture->modifyAlpha(-100);

	// Player to Enemy unit direction vector
	VectorF enemyPosition = mEnemy->getMovement().getPostion();
	VectorF source = mEnemy->getData()->player->getRect().Center();
	VectorF hitDirection = (enemyPosition - source).normalise();

	mEnemy->getMovement().setDirection(hitDirection);

	const Damage damage = mEnemy->getCollider()->getOtherColliderDamage();
	mEnemy->health().takeDamage(damage.get());
	printf("add HIT: enemy taking %f damage, health now %f\n", damage.get(), mEnemy->health().get());
}

void EnemyHit::slowUpdate(float dt)
{
	if (timer.getSeconds() > mEnemy->hurtTime())
	{
		//mEnemy->getStateMachine()->popState();

		mEnemy->replaceState(EnemyState::Run);
	}
	if (mEnemy->health().isDead())
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

	mEnemy->getCollider()->didCollide(false);
	//mEnemy->getCollider()->setActive(true);

	mEnemy->getMovement().setDirection(VectorF());
	printf("exit hit\n");
}