#include "pch.h"
#include "EnemyAttack.h"

#include "Characters/Enemies/Enemy.h"


EnemyAttack::EnemyAttack(Enemy* enemy) :
	mEnemy(enemy), 
	mAttackDistance(0.0f), 
	mHasAttacked(false),
	mIsAttacking(false)
{ }


void EnemyAttack::init()
{
	printf("attack\n");
	mAttackDistance = 0.0f;
	mHasAttacked = false;
	mIsAttacking = false;

	mEnemy->getAnimator()->selectAnimation("Attack");
	timer.restart();

	VectorF direction = mEnemy->targetRect().Center() - mEnemy->getRect().Center();
	mEnemy->getMovement().setDirection(direction);

	mEnemy->getMovement().setSpeed(mEnemy->tackleSpeed());
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
	mEnemy->getMovement().setSpeed(mEnemy->movementSpeed());
}


// --- Private Functions ---
bool EnemyAttack::beginMovement() const
{
	return !mIsAttacking && !mHasAttacked && (timer.getSeconds() > mEnemy->tackleChargeTime());
}


bool EnemyAttack::returnMovement() const
{
	return mIsAttacking && !mHasAttacked && (mAttackDistance > mEnemy->tackleDistance());
}


bool EnemyAttack::endAttack() const
{
	return mHasAttacked && mAttackDistance <= 0.0f;
}