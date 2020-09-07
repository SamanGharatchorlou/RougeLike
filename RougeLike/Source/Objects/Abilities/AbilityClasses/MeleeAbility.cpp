#include "pch.h"
#include "MeleeAbility.h"

#include "Input/InputManager.h"
#include "Collisions/Colliders/QuadCollider.h"

void MeleeAbility::init()
{
	mCollider = new QuadCollider(&mQuad);
}

bool MeleeAbility::initiate(const InputManager* input) const
{
	return input->isCursorReleased(Cursor::Left);
}


bool MeleeAbility::activateOn(Actor* target, EffectPool* effectPool)
{
	if (mHitList.count(target) == 0)
	{
		applyEffects(target, effectPool);
		mHitList.insert(target);
		return true;
	}

	return false;
}


void MeleeAbility::baseExit()
{
	Ability::baseExit();
	mHitList.clear();
}