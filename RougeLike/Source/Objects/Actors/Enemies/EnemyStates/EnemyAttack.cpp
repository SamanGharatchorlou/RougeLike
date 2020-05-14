#include "pch.h"
#include "EnemyAttack.h"

#include "Collisions/Collider.h"
#include "Objects/Actors/Enemies/Enemy.h"


EnemyAttack::EnemyAttack(Enemy* enemy) :
	EnemyState(enemy),
	mHasAttacked(false),
	hitCounter(0)
{ 
	mEnemy->collider()->setDidHit(false);
}


void EnemyAttack::init()
{ 
	startingPosition = mEnemy->position();
	attackTargetPosition = mEnemy->attackTargetRect()->Center();

	// Face player
	mEnemy->physics()->facePoint(attackTargetPosition);
}


void EnemyAttack::fastUpdate(float dt)
{
	if (mHasAttacked)
	{
		VectorF direction = startingPosition - mEnemy->position();
		VectorF velocity = direction.normalise() * mEnemy->getPropertyValue("TackleMovementSpeed") / 1.5f;
		mEnemy->move(velocity, dt);
	}
	else
	{
		VectorF direction = attackTargetPosition - mEnemy->position();
		VectorF velocity = direction.normalise() * mEnemy->getPropertyValue("TackleMovementSpeed");
		mEnemy->move(velocity, dt);
	}
}


void EnemyAttack::slowUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();

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
	if (mEnemy->collider()->didHit() || hitCounter > 0)
		hitCounter++;

	if (!mHasAttacked)
	{
		// Maximum attack distance
		float distanceTravelled = distanceSquared(startingPosition, mEnemy->position());
		if (distanceTravelled >= mEnemy->getPropertyValue("TackleDistance"))
			mHasAttacked = true;

		if (hitCounter >= 5)
			mHasAttacked = true;

		if (distanceSquared(attackTargetPosition, mEnemy->position()) < 5.0f)
			mHasAttacked = true;
	}
}


bool EnemyAttack::attackComplete() const
{
	return distanceSquared(startingPosition, mEnemy->position()) < 5.0f ? mHasAttacked : false;
}