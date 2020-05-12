#include "pch.h"
#include "EnemyPreAttack.h"

#include "Objects/Enemies/Enemy.h"
#include "Collisions/Collider.h"

EnemyPreAttack::EnemyPreAttack(Enemy* enemy) : EnemyState(enemy)
{
	mEnemy->collider()->setDidHit(false);
}


void EnemyPreAttack::init()
{
	mEnemy->animator().selectAnimation("Attack");
	timer.restart();
}


void EnemyPreAttack::slowUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();

	if (!inAttackRange())
		mEnemy->popState();

	// begin attack
	if (timer.getSeconds() > (1.0f / mEnemy->getPropertyValue("AttackSpeed")))
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