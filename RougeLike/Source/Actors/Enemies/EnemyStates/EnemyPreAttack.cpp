#include "pch.h"
#include "EnemyPreAttack.h"

#include "Actors/Enemies/Enemy.h"
#include "Collisions/Colliders/Collider.h"


#include "Objects/Abilities/AbilityClasses/Ability.h"

void EnemyPreAttack::enter()
{
	mEnemy->collider()->setDidHit(false);
}


void EnemyPreAttack::init()
{
	//printf("init pre\n");
	mEnemy->animator().selectAnimation(Action::Attack);
	timer.restart();
}


void EnemyPreAttack::slowUpdate(float dt)
{
	if (!inAttackRange())
		mEnemy->popState();

	if (timer.getSeconds() > (1.0f / mEnemy->getAttributeValue(AttributeType::AttackSpeed)))
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
	mEnemy->collider()->setDidHit(false);
	init();
}

