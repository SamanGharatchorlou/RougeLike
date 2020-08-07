#include "pch.h"
#include "EnemyPreAttack.h"

#include "Actors/Enemies/Enemy.h"
#include "Collisions/Collider.h"

EnemyPreAttack::EnemyPreAttack(Enemy* enemy) : EnemyState(enemy)
{
	mEnemy->collider()->setDidHit(false);
}


void EnemyPreAttack::init()
{
	mEnemy->animator().selectAnimation(Action::Attack);
	timer.restart();
}


void EnemyPreAttack::slowUpdate(float dt)
{
	mEnemy->resolveCollisions();

	if (!inAttackRange())
		mEnemy->popState();

	// begin attack
	if (timer.getSeconds() > (1.0f / mEnemy->getAttributeValue(AttributeType::AttackSpeed)))
		mEnemy->replaceState(EnemyState::Attack);
}


void EnemyPreAttack::render()
{
	mEnemy->renderCharacter();
}


bool EnemyPreAttack::inAttackRange() const
{
	VectorF currentPosition = mEnemy->position();
	VectorF nearestTargetSide = closestRectSide(currentPosition, mEnemy->target()->scaledRect());

	return distance(currentPosition, nearestTargetSide) < (mEnemy->getAttributeValue(AttributeType::TackleDistance));
}

void EnemyPreAttack::resume()
{
	mEnemy->collider()->setDidHit(false);
	init();
}

