#include "pch.h"
#include "EnemyAttack.h"

#include "Actors/Enemies/Enemy.h"


void EnemyAttack::init()
{
	initProperties();

	if (mEnemy->hasTarget())
	{
		VectorF attackPosition = mEnemy->target()->position();
		mStartPosition = mEnemy->position();
		mDirection = (attackPosition - mStartPosition).normalise();
		mEndPosition = mStartPosition + mDirection * mEnemy->getAttributeValue(AttributeType::TackleDistance);

		mEnemy->physics()->facePoint(attackPosition);
		mEnemy->animator().selectAnimation(Animation::Idle);
	}
	else
	{
		mHasAttacked = true;
	}
}

void EnemyAttack::initProperties()
{
	mHasAttacked = false;
	mDidHit = false;
	mEnemy->collider()->setDidHit(false);

	mStartPosition.zero();
	mEndPosition.zero();
	mDirection.zero();
}


void EnemyAttack::fastUpdate(float dt)
{
	VectorF velocity = mDirection * mEnemy->getAttributeValue(AttributeType::TackleMovementSpeed);
	if (mHasAttacked)
	{
		// Flip direction and reduce speed
		velocity /= -1.5f;
	}

	mEnemy->move(velocity, dt);

	if (mEnemy->collider()->didHit())
		mDidHit = true;
}


void EnemyAttack::slowUpdate(float dt)
{
	// Return to starting position
	updateHasAttackedStatus();

	// End attack
	if (mHasAttacked && attackComplete())
		mEnemy->popState();
}


void EnemyAttack::render()
{
	mEnemy->renderCharacter();

#if DRAW_ENEMY_TARGET_PATH
	debugDrawPoint(mStartPosition, 10.0f, RenderColour::Blue);
	debugDrawLine(mStartPosition, mEndPosition, RenderColour::Green);
	debugDrawPoint(mEndPosition, 10.0f, RenderColour::Red);
#endif
}


// --- Private Functions ---

void EnemyAttack::updateHasAttackedStatus()
{
	if (!mHasAttacked)
	{
		VectorF currentDirection = (mEndPosition - mEnemy->position());
		bool hasPassedTarget = !((currentDirection.x > 0 == mDirection.x > 0) && (currentDirection.y > 0 == mDirection.y > 0));
		if (hasPassedTarget)
			mHasAttacked = true;
	}
}


bool EnemyAttack::attackComplete() const
{
	VectorF currentDirection = (mStartPosition - mEnemy->position());
	bool hasPassedTarget = !((currentDirection.x > 0 == -mDirection.x > 0) && (currentDirection.y > 0 == -mDirection.y > 0));
	return hasPassedTarget;
}


void EnemyAttack::resume()
{
	init();
}



void EnemyAttack::exit()
{
	initProperties();
}