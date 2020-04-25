#include "pch.h"
#include "EnemyAttack.h"

#include "Characters/Enemies/Enemy.h"
#include "Characters/Enemies/EnemyPropertyBag.h"

EnemyAttack::EnemyAttack(Enemy* enemy) :
	EnemyState(enemy),
	mHasAttacked(false),
	hitCounter(0)
{ 
	mEnemy->getCollider()->setDidHit(false);
}


void EnemyAttack::init()
{ 
	startingPosition = mEnemy->position();
	attackTargetPosition = mEnemy->attackTargetRect()->Center();

	// Face player
	mEnemy->facePoint(attackTargetPosition);
}


void EnemyAttack::fastUpdate(float dt)
{
	if (mHasAttacked)
	{
		VectorF direction = startingPosition - mEnemy->position();
		VectorF velocity = direction.normalise() * mEnemy->propertyBag().pTackleSpeed.get() / 1.5f;
		mEnemy->move(velocity, dt);
	}
	else
	{
		VectorF direction = attackTargetPosition - mEnemy->position();
		VectorF velocity = direction.normalise() * mEnemy->propertyBag().pTackleSpeed.get();
		mEnemy->move(velocity, dt);
	}
	
	mEnemy->resolvePlayerWeaponCollisions();
}


void EnemyAttack::slowUpdate(float dt)
{
	// Return to starting position
	updateHasAttackedStatus();

	// End attack
	if (attackComplete() == true)
		mEnemy->popState();
}


void EnemyAttack::render()
{
	mEnemy->renderCharacter();

#if DRAW_ENEMY_TARGET_PATH
	VectorF targetPosition = mHasAttacked ? startingPosition : attackTargetPosition;
	debugDrawLine(mEnemy->position(), targetPosition, RenderColour::Red);
#endif
}


// --- Private Functions ---

void EnemyAttack::updateHasAttackedStatus()
{
	if (mEnemy->getCollider()->didHit() || hitCounter > 0)
		hitCounter++;

	if (!mHasAttacked)
	{
		// Maximum attack distance
		float distanceTravelled = distanceSquared(startingPosition, mEnemy->position());

		if (distanceTravelled >= mEnemy->propertyBag().pTackleDistance.get())
			mHasAttacked = true;

		if (hitCounter >= 5)
			mHasAttacked = true;


		float distanceToAttackTarget = distanceSquared(attackTargetPosition, mEnemy->position());

		if (distanceToAttackTarget < 5.0f)
			mHasAttacked = true;
	}
}


bool EnemyAttack::attackComplete() const
{
	return distanceSquared(startingPosition, mEnemy->position()) < 5.0f ? mHasAttacked : false;
}