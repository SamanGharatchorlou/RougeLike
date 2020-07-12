#include "pch.h"
#include "EnemyAttack.h"

#include "Collisions/Collider.h"
#include "Objects/Actors/Enemies/Enemy.h"

#include "Objects/Effects/EffectTypes/DisplacementEffect.h"
#include "Objects/Effects/EffectTypes/DamageEffect.h"


EnemyAttack::EnemyAttack(Enemy* enemy) :
	EnemyState(enemy),
	mHasAttacked(false),
	hitCounter(0)
{ 
	mEnemy->collider()->setDidHit(false);
}


void EnemyAttack::init()
{
	mStartPosition = mEnemy->position();
	mAttackPosition = mEnemy->target()->position();
	mEnemy->physics()->facePoint(mAttackPosition);
	addEffects();
}


void EnemyAttack::fastUpdate(float dt)
{
	if (mHasAttacked)
	{
		VectorF direction = mStartPosition - mEnemy->position();
		VectorF velocity = direction.normalise() * mEnemy->getPropertyValue("TackleMovementSpeed") / 1.5f;
		mEnemy->move(velocity, dt);
	}
	else
	{
		VectorF direction = mAttackPosition - mEnemy->position();
		VectorF velocity = direction.normalise() * mEnemy->getPropertyValue("TackleMovementSpeed");
		mEnemy->move(velocity, dt);
	}
}


void EnemyAttack::slowUpdate(float dt)
{
	updateEffects();

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
	if (mEnemy->collider()->didHit() || hitCounter > 0)
		hitCounter++;

	if (!mHasAttacked)
	{
		// Maximum attack distance
		float distanceTravelled = distanceSquared(mStartPosition, mEnemy->position());
		if (distanceTravelled >= mEnemy->getPropertyValue("TackleDistance"))
			mHasAttacked = true;

		if (hitCounter >= 5)
			mHasAttacked = true;

		if (distanceSquared(mAttackPosition, mEnemy->position()) < 5.0f)
			mHasAttacked = true;
	}
}


bool EnemyAttack::attackComplete() const
{
	return distanceSquared(mStartPosition, mEnemy->position()) < 5.0f ? mHasAttacked : false;
}


void EnemyAttack::resume()
{
	mHasAttacked = false;
	hitCounter = 0;
	mEnemy->collider()->setDidHit(false);
	init();
}


void EnemyAttack::addEffects()
{
	Effect* dmgEffect = mEnemy->getEffectFromPool(EffectType::Damage);
	DamageEffect* damageEffect = static_cast<DamageEffect*>(dmgEffect);
	damageEffect->set(mEnemy->getPropertyValue("Damage"));

	Effect* displaceEffect = mEnemy->getEffectFromPool(EffectType::Displacement);
	DisplacementEffect* displacementEffect = static_cast<DisplacementEffect*>(displaceEffect);

	VectorF source; // to be updated
	float force = mEnemy->getPropertyValue("KnockbackForce");
	float distance = mEnemy->getPropertyValue("KnockbackDistance");
	displacementEffect->set(source, force, distance);

	mEnemy->collider()->addEffect(damageEffect);
	mEnemy->collider()->addEffect(displacementEffect);
}


void EnemyAttack::updateEffects()
{
	EffectCollider* collider = mEnemy->collider();

	for (int i = 0; i < collider->effectCount(); i++)
	{
		Effect* effect = collider->popEffect();

		if (effect->type() == EffectType::Displacement)
		{
			DisplacementEffect* displacementEffect = static_cast<DisplacementEffect*>(effect);
			displacementEffect->update(mEnemy->position());
		}

		collider->addEffect(effect);
	}
}