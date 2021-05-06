#include "pch.h"

#include "RangedAttackAbility.h"

#include "Actors/Actor.h"
#include "Objects/Effects/EffectTypes/EffectTypes.h"


void RangedAttackAbility::activate()
{
	mCaster->meleeAttack();
}


void RangedAttackAbility::slowUpdate(float dt)
{
	if (!mCaster->isAttacking())
	{
		mCompleted = true;
	}
}

void RangedAttackAbility::applyEffects(Actor* actor, EffectPool* effectPool)
{
	applyEffect(EffectType::Damage, actor, effectPool);
}