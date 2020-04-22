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
	
	startingPosition = mEnemy->position();
	attackTargetPosition = mEnemy->attackTargetRect()->Center();
}


void EnemyAttack::init()
{
	mEnemy->physics().setForce(mEnemy->physics().force() * 4.0f);
	mEnemy->physics().setMaxVelocity(mEnemy->physics().maxVelocity() * 4.0f);
}


void EnemyAttack::fastUpdate(float dt)
{
	if (mHasAttacked)
	{
		mEnemy->moveTowards(startingPosition);
		
	}
	else
	{
		mEnemy->moveTowards(attackTargetPosition);
	}
	
	mEnemy->resolvePlayerWeaponCollisions();
}


void EnemyAttack::slowUpdate(float dt)
{


	// Return to starting position
	mHasAttacked = forwardAttackComplete();

	if (mHasAttacked)
	{
		printf("back to start\n");

	}
	else
	{
		printf("attack\n");
	}





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


void EnemyAttack::exit()
{
	mEnemy->physics().setForce(mEnemy->physics().force() / 4.0f);
	mEnemy->physics().setMaxVelocity(mEnemy->physics().maxVelocity() / 4.0f);

}


// --- Private Functions ---

bool EnemyAttack::forwardAttackComplete()
{
	if (!mHasAttacked)
	{
		float distance = distanceSquared(startingPosition, mEnemy->position());


		// Maximum attack distance
		if (distance >= mEnemy->propertyBag().pTackleDistance.get())
			return true;

		// Attack connected, early exit
		if (mEnemy->getCollider()->didHit())
			hitCounter++;

		// Prevent the enemy returning too early from just touching the player, give it a few frames
		if(hitCounter >= 10)
			return true;

		return false;
	}
	else
		return true;

	//return false;
}


bool EnemyAttack::attackComplete() const
{
	bool returnValue = false;
	float distance = distanceSquared(startingPosition, mEnemy->position());

	if (distance < 0.1f)
		returnValue = mHasAttacked;// && (mAttackDistance <= 0.0f);
	else
		returnValue = false;

	return returnValue;
}