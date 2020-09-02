#include "pch.h"
#include "EnemyPreAttack.h"

#include "Actors/Enemies/Enemy.h"
#include "Objects/Abilities/AbilityClasses/Ability.h"


void EnemyPreAttack::enter()
{
}


void EnemyPreAttack::init()
{
	mEnemy->animator().selectAnimation(Action::Attack);
	timer.restart();

	float attackWaitTime = 1.0f / mEnemy->getAttributeValue(AttributeType::AttackSpeed);
	float range = attackWaitTime / 4.0f;
	mAttackWaitTime = randomNumberBetween(attackWaitTime - range, attackWaitTime + range);
}


void EnemyPreAttack::slowUpdate(float dt)
{
	if (!inAttackRange())
		mEnemy->popState();

	if (timer.getSeconds() > mAttackWaitTime)
	{
		Ability* basicAttack = mEnemy->abilities().get(AbilityType::Attack);
		if (basicAttack->state() == AbilityState::Idle)
		{
			basicAttack->activate(VectorF());
			basicAttack->setState(AbilityState::Running);
		}
	}
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
	init();
}

