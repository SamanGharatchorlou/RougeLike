#include "pch.h"
#include "EnemyCharge.h"

#include "Actors/Enemies/Enemy.h"


void EnemyCharge::init()
{
	initProperties();

	if (mEnemy->hasTarget())
	{
		VectorF target = mEnemy->target()->position();
		mStartPosition = mEnemy->position();
		mDirection = (target - mStartPosition).normalise();

		mEnemy->physics()->facePoint(target);
		mEnemy->animator().selectAnimation(Animation::Attack);
	}
	else
	{
		mDirection = VectorF(0.0f, 0.0f);
		mEndAttack = true;
	}
}


void EnemyCharge::fastUpdate(float dt)
{
	VectorF velocity = mDirection * mEnemy->getAttributeValue(AttributeType::TackleMovementSpeed);
	velocity = mWallCollisions.allowedVelocity(mEnemy->currentMap(), velocity, dt);

	if (velocity.x == 0.0f || velocity.y == 0.0f)
		mStunned = true;

	mEnemy->move(velocity, dt);

	if (mEnemy->collider()->didHit())
		mEndAttack = true;
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
	VectorF target = mStartPosition + (mDirection * mEnemy->getAttributeValue(AttributeType::TackleDistance));
	debugDrawLine(mEnemy->position(), target, RenderColour::Red);
#endif
}


void EnemyCharge::resume()
{
	mEndAttack = true;
}


void EnemyCharge::exit()
{
	initProperties();
}


// -- Private Functions -- //

void EnemyCharge::initProperties()
{
	mWallCollisions.setCollider(mEnemy->collider());
	mEnemy->collider()->setDidHit(false);

	mEndAttack = false;
	mStunned = false;

	mStartPosition.zero();
	mDirection.zero();
}

void EnemyCharge::updateHasAttackedStatus()
{
	if (!mEndAttack)
	{
		// Maximum attack distance
		float distanceTravelled = distance(mStartPosition, mEnemy->position());
		if (distanceTravelled >= mEnemy->getAttributeValue(AttributeType::TackleDistance))
		{
			mEndAttack = true;
		}
	}
}