#include "pch.h"
#include "EnemyAttack.h"

#include "Collisions/Colliders/EffectCollider.h"
#include "Actors/Enemies/Enemy.h"

#include "Objects/Effects/EffectTypes/DisplacementEffect.h"
#include "Objects/Effects/EffectTypes/DamageEffect.h"


EnemyAttack::EnemyAttack() : mHasAttacked(false), mHitCounter(0)
{ 
}

void EnemyAttack::enter()
{
	mEnemy->collider()->setDidHit(false);
}

void EnemyAttack::init()
{
	mStartPosition = mEnemy->position();
	mAttackPosition = mEnemy->target()->position();
	mEnemy->physics()->facePoint(mAttackPosition);

	if (!mEnemy->collider()->hasEffects())
	{
		UniqueQueue<Effect*> effects = mEnemy->effects().getNewAttackingEffects();
		mEnemy->collider()->addNewEffects(effects);
	}

	// TODO: I think it should work just fine only setting the source position
	// at the start of the attack? in terms of vector directions seems to make more sense
	VectorF position = mEnemy->position();
	mEnemy->effects().attackingData().addXYPosition(position);
	mEnemy->collider()->updateEffectData(mEnemy->effects().attackingData());
}


void EnemyAttack::fastUpdate(float dt)
{
	if (mHasAttacked)
	{
		VectorF direction = mStartPosition - mEnemy->position();
		VectorF velocity = direction.normalise() * mEnemy->getAttributeValue(AttributeType::TackleMovementSpeed) / 1.5f;
		mEnemy->move(velocity, dt);
	}
	else
	{
		VectorF direction = mAttackPosition - mEnemy->position();
		VectorF velocity = direction.normalise() * mEnemy->getAttributeValue(AttributeType::TackleMovementSpeed);
		mEnemy->move(velocity, dt);
	}
}


void EnemyAttack::slowUpdate(float dt)
{
	mEnemy->resolveCollisions();

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
	if (mEnemy->collider()->didHit() || mHitCounter > 0)
		mHitCounter++;

	if (!mHasAttacked)
	{
		// Maximum attack distance
		float distanceTravelled = distance(mStartPosition, mEnemy->position());
		if (distanceTravelled >= mEnemy->getAttributeValue(AttributeType::TackleDistance))
			mHasAttacked = true;

		if (mHitCounter >= 5)
			mHasAttacked = true;

		if (distance(mAttackPosition, mEnemy->position()) < 5.0f)
			mHasAttacked = true;
	}
}


bool EnemyAttack::attackComplete() const
{
	float smallDistance = 5.0f;
	return distance(mStartPosition, mEnemy->position()) < smallDistance ? mHasAttacked : false;
}


void EnemyAttack::resume()
{
	mHasAttacked = false;
	mHitCounter = 0;
	mEnemy->collider()->setDidHit(false);
	init();
}



void EnemyAttack::exit()
{
	mHasAttacked = false;
	mHitCounter = 0;
}