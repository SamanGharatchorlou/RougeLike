#include "pch.h"
#include "EnemyPreAttack.h"

#include "Characters/Enemies/Enemy.h"

EnemyPreAttack::EnemyPreAttack(Enemy* enemy) : EnemyState(enemy)
{
	mEnemy->getCollider()->setDidHit(false);
}


void EnemyPreAttack::init()
{
	mEnemy->getAnimator()->selectAnimation("Attack");
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
	if (timer.getSeconds() > mEnemy->propertyBag().pTackleChargeTime.get())
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

	return distanceSquared(currentPosition, nearestTargetSide) < (mEnemy->propertyBag().pTackleDistance.get());
}