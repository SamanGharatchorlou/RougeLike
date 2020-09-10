#include "pch.h"
#include "EnemyPreAttack.h"

#include "Actors/Enemies/Enemy.h"
#include "Objects/Abilities/AbilityClasses/Ability.h"


void EnemyPreAttack::init()
{
	mEnemy->animator().selectAnimation(Action::Idle);
	timer.restart();

	float attackWaitTime = 100.0f / mEnemy->getAttributeValue(AttributeType::AttackSpeed);
	float range = attackWaitTime / 4.0f;
	mAttackWaitTime = (float)randomNumberBetween((int)(attackWaitTime - range), (int)(attackWaitTime + range)) / 100.0f;
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
			basicAttack->activate();
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
	if (mEnemy->hasTarget())
	{
		VectorF currentPosition = mEnemy->position();
		VectorF nearestTargetSide = closestRectSide(currentPosition, mEnemy->target()->scaledRect());

		return distance(currentPosition, nearestTargetSide) < (mEnemy->getAttributeValue(AttributeType::TackleDistance));
	}
	else
	{
		return false;
	}

}

void EnemyPreAttack::resume()
{
	init();
}

