#include "pch.h"
#include "EnemyAttack.h"

#include "Collisions/Collider.h"
#include "Actors/Enemies/Enemy.h"

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

	if(!mEnemy->collider()->hasEffects())
		addEffects();
}


void EnemyAttack::fastUpdate(float dt)
{
	VectorF direction = mStartPosition - mEnemy->position();
	VectorF velocity = direction.normalise() * mEnemy->getAttributeValue(AttributeType::TackleMovementSpeed);

	if (mHasAttacked)
		velocity /= 1.5f;

	mEnemy->move(velocity, dt);
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
		float distanceTravelled = distance(mStartPosition, mEnemy->position());
		if (distanceTravelled >= mEnemy->getAttributeValue(AttributeType::TackleDistance))
			mHasAttacked = true;

		if (hitCounter >= 5)
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
	hitCounter = 0;
	mEnemy->collider()->setDidHit(false);
	init();
}


void EnemyAttack::addEffects()
{
	std::queue<Effect*> effects = mEnemy->effects().getAttackingEffects();

	while (effects.size() > 0)
	{
		Effect* effect = effects.front();
		effects.pop();

		mEnemy->collider()->addEffect(effect);
	}
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


void EnemyAttack::exit()
{

}