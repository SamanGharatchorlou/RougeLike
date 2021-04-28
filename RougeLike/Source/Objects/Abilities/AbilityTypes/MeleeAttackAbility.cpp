#include "pch.h"
#include "MeleeAttackAbility.h"

#include "Actors/Actor.h"
#include "Objects/Effects/EffectTypes/EffectTypes.h"


// MeleeAttack never owns its collider
MeleeAttackAbility::~MeleeAttackAbility()
{ 
	mCollider = nullptr; 
}



void MeleeAttackAbility::activate()
{
	mCollider = mCaster->attackingCollider();
	mActivateCollisions = true;
	mCaster->meleeAttack();
}


void MeleeAttackAbility::slowUpdate(float dt)
{
	if (!mCaster->isAttacking())
	{
		mCompleted = true;
	}
}


void MeleeAttackAbility::applyEffects(Actor* actor, EffectPool* effectPool)
{
	mProperties.addXYPosition(mCaster->position());

	applyEffect(EffectType::Damage, actor, effectPool);
	applyEffect(EffectType::Displacement, actor, effectPool);
}

void MeleeAttackAbility::exit()
{
	mCollider = nullptr;
}