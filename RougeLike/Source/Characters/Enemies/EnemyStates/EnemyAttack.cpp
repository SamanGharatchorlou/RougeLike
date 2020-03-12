#include "pch.h"
#include "EnemyAttack.h"

#include "Characters/Enemies/Enemy.h"
#include "Characters/Enemies/EnemyPropertyBag.h"

EnemyAttack::EnemyAttack(Enemy* enemy) :
	mEnemy(enemy), 
	mAttackDistance(0.0f), 
	mHasAttacked(false)
{ 
	mEnemy->setActive(true);
	mEnemy->getCollider()->setDidHit(false);
}


void EnemyAttack::init()
{
	timer.restart();
	mEnemy->getMovement().setSpeed(mEnemy->propertyBag().pTackleSpeed.get());
}


void EnemyAttack::slowUpdate(float dt)
{
	// Return to starting position
	if (returnMovement())
	{
		mEnemy->getMovement().flipDirection();
		mHasAttacked = true;
	}

	// End attack
	if (endAttack())
	{
		mEnemy->popState();
	}
}


void EnemyAttack::fastUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();

	int attackDirection = mHasAttacked ? -1 : +1;
	mAttackDistance += attackDirection * distanceSquared(VectorF(), mEnemy->getMovement().getMovementDistance()) * dt;

	mEnemy->move(dt);
}


void EnemyAttack::exit()
{
	// TODO: using int movement speed for float in mMovement class, fix me
	mEnemy->getMovement().setSpeed((float)mEnemy->propertyBag().pMovementSpeed.get());
}


// --- Private Functions ---

bool EnemyAttack::returnMovement() const
{
	return !mHasAttacked && (mAttackDistance > mEnemy->propertyBag().pTackleDistance.get());
}


bool EnemyAttack::endAttack() const
{
	return mHasAttacked && mAttackDistance <= 0.0f;
}