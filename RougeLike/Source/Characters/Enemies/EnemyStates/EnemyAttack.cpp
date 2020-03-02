#include "pch.h"
#include "EnemyAttack.h"

#include "Characters/Enemies/Enemy.h"
#include "Characters/Enemies/EnemyPropertyBag.h"

EnemyAttack::EnemyAttack(Enemy* enemy) :
	mEnemy(enemy), 
	mAttackDistance(0.0f), 
	mHasAttacked(false),
	mIsAttacking(false)
{ }


void EnemyAttack::init()
{
	mAttackDistance = 0.0f;
	mHasAttacked = false;
	mIsAttacking = false;

	mEnemy->setActive(true);
	mEnemy->getCollider()->hasProcessedAttack(false);

	mEnemy->getAnimator()->selectAnimation("Attack");
	timer.restart();

	VectorF direction = mEnemy->targetRect().Center() - mEnemy->getRect().Center();
	mEnemy->getMovement().setDirection(direction);
	mEnemy->getMovement().setSpeed(mEnemy->propertyBag().pTackleSpeed.get());
}


void EnemyAttack::slowUpdate(float dt)
{
	// Start attack movement
	if (beginMovement())
	{
		mIsAttacking = true;
	}

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


	if (mEnemy->getCollider()->hasCollided())
	{
		mEnemy->getCollider()->hasProcessedAttack(true);
	}
}


void EnemyAttack::fastUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();

	if (mIsAttacking)
	{
		int attackDirection = mHasAttacked ? -1 : +1;
		mAttackDistance += attackDirection * distanceSquared(VectorF(), mEnemy->getMovement().getMovementDistance()) * dt;
	}

	if(mIsAttacking)
		mEnemy->move(dt);
}


void EnemyAttack::exit()
{
	// TODO: using int movement speed for float in mMovement class, fix me
	mEnemy->getMovement().setSpeed((float)mEnemy->propertyBag().pMovementSpeed.get());
}


// --- Private Functions ---
bool EnemyAttack::beginMovement() const
{
	return !mIsAttacking && !mHasAttacked && (timer.getSeconds() > mEnemy->propertyBag().pTackleChargeTime.get());
}


bool EnemyAttack::returnMovement() const
{
	return mIsAttacking && !mHasAttacked && (mAttackDistance > mEnemy->propertyBag().pTackleDistance.get());
}


bool EnemyAttack::endAttack() const
{
	return mHasAttacked && mAttackDistance <= 0.0f;
}