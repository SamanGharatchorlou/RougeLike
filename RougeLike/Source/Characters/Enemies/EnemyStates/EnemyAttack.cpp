#include "pch.h"
#include "EnemyAttack.h"

#include "Characters/Enemies/Enemy.h"
#include "Characters/Enemies/EnemyPropertyBag.h"

EnemyAttack::EnemyAttack(Enemy* enemy) :
	EnemyState(enemy),
	mAttackDistance(0.0f), 
	mHasAttacked(false),
	hitCounter(0)
{ 
	mEnemy->getCollider()->setDidHit(false);
}


void EnemyAttack::init()
{
	timer.restart();
	mEnemy->getMovement().setSpeed(mEnemy->propertyBag().pTackleSpeed.get());

	startingPosition = mEnemy->getMovement().getPostion();
}


void EnemyAttack::fastUpdate(float dt)
{
	int attackDirection = mHasAttacked ? -1 : +1;
	mAttackDistance += attackDirection * distanceSquared(VectorF(), mEnemy->getMovement().getMovementDistance()) * dt;

	mEnemy->move(dt);

	mEnemy->resolvePlayerWeaponCollisions();
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
		mEnemy->popState();
}


void EnemyAttack::render()
{
	mEnemy->renderCharacter();
}


void EnemyAttack::exit()
{
	// TODO: using int movement speed for float in mMovement class, fix me
	mEnemy->getMovement().setSpeed((float)mEnemy->propertyBag().pMovementSpeed.get());
}


// --- Private Functions ---

bool EnemyAttack::returnMovement()
{
	if (!mHasAttacked)
	{
		float distance = distanceSquared(startingPosition, mEnemy->getMovement().getPostion());

		if (distance >= mEnemy->propertyBag().pTackleDistance.get())
			return true;

		if (mEnemy->getCollider()->didHit())
			hitCounter++;

		if(hitCounter >= 10)
			return true;
	}

	return false;
}


bool EnemyAttack::endAttack() const
{
	return mHasAttacked && mAttackDistance <= 0.0f;
}