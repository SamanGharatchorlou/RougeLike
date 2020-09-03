#include "pch.h"
#include "BasicAttackAbility.h"

#include "Actors/Actor.h"
#include "Objects/Effects/EffectTypes/EffectTypes.h"


// BasicAttack never owns its collider
BasicAttackAbility::~BasicAttackAbility()
{ 
	mCollider = nullptr; 
}



void BasicAttackAbility::activate(VectorF position)
{
	mCollider = mCaster->attackingCollider();
	mActivateCollisions = true;
	mCaster->attack();
}


void BasicAttackAbility::slowUpdate(float dt)
{
	if (!mCaster->isAttacking())
	{
		mCompleted = true;
	}
}


void BasicAttackAbility::applyEffects(Actor* actor, EffectPool* effectPool)
{
	mProperties.addXYPosition(mCaster->position());

	applyEffect(EffectType::Damage, actor, effectPool);
	applyEffect(EffectType::Displacement, actor, effectPool);
}

void BasicAttackAbility::exit()
{
	mCollider = nullptr;
}