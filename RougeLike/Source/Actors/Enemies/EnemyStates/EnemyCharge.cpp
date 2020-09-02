#include "pch.h"
#include "EnemyCharge.h"

#include "Actors/Enemies/Enemy.h"

void EnemyCharge::enter()
{
	mWallCollisions.setActor(mEnemy);
	mEnemy->collider()->setDidHit(false);

	mEndAttack = false;
	mStunned = false;
}


void EnemyCharge::init()
{
	mStartPosition = mEnemy->position();
	VectorF target = mEnemy->target()->position();
	mDirection = (target - mStartPosition).normalise();

	mEnemy->physics()->facePoint(target);
	mEnemy->animator().selectAnimation(Action::Attack);

	//if (inAttackRange())
	//	mEnemy->replaceState(EnemyState::Attack);
}


void EnemyCharge::fastUpdate(float dt)
{
	VectorF velocity = mDirection * mEnemy->getAttributeValue(AttributeType::TackleMovementSpeed);
	velocity = mWallCollisions.allowedVelocity(mEnemy->currentMap(), velocity, dt);

	if (velocity.x == 0.0f || velocity.y == 0.0f)
	{
		mStunned = true;
	}

	mEnemy->move(velocity, dt);
}


void EnemyCharge::slowUpdate(float dt)
{
	// Return to starting position
	updateHasAttackedStatus();

	// End attack
	if (mEndAttack)
		mEnemy->popState();
	else if (mStunned)
		mEnemy->stun(2.0f);
}


void EnemyCharge::render()
{
	mEnemy->renderCharacter();

#if DRAW_ENEMY_TARGET_PATH
	VectorF targetPosition = mHasAttacked ? startingPosition : attackTargetPosition;
	debugDrawLine(mEnemy->position(), targetPosition, RenderColour::Red);
#endif
}


void EnemyCharge::updateHasAttackedStatus()
{
	if (mEnemy->collider()->didHit() || mHitCounter > 0)
		mHitCounter++;

	if (!mEndAttack)
	{
		// Maximum attack distance
		float distanceTravelled = distance(mStartPosition, mEnemy->position());
		if (distanceTravelled >= mEnemy->getAttributeValue(AttributeType::TackleDistance))
		{
			mEndAttack = true;
		}

		if (mHitCounter >= 5)
			mEndAttack = true;
	}
}



void EnemyCharge::resume()
{
	mEndAttack = true;
	//mEnemy->popState();
}


void EnemyCharge::exit()
{
	mEndAttack = false;
	mStunned = false;
	mHitCounter = 0;
}