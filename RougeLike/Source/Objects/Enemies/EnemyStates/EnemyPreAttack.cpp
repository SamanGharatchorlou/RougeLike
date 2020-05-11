#include "pch.h"
#include "EnemyPreAttack.h"

#include "Objects/Enemies/Enemy.h"

EnemyPreAttack::EnemyPreAttack(Enemy* enemy) : EnemyState(enemy)
{
	mEnemy->collider()->setDidHit(false);
}


void EnemyPreAttack::init()
{
	mEnemy->animator().selectAnimation("Attack");
	timer.restart();
}

void EnemyPreAttack::fastUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();
}


void EnemyPreAttack::slowUpdate(float dt)
{
	if (!inAttackRange())
		mEnemy->popState();

	// begin attack
	if (timer.getSeconds() > mEnemy->getPropertyValue("TackleChargeTime"))
		mEnemy->replaceState(EnemyState::Attack);
}


void EnemyPreAttack::render()
{
	mEnemy->renderCharacter();
}


bool EnemyPreAttack::inAttackRange() const
{
	VectorF currentPosition = mEnemy->position();
	VectorF nearestTargetSide = closestRectSide(currentPosition, *mEnemy->attackTargetRect());

	return distanceSquared(currentPosition, nearestTargetSide) < (mEnemy->getPropertyValue("TackleDistance"));
}