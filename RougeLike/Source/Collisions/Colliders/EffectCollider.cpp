#include "pch.h"
#include "EffectCollider.h"


Effect* EffectCollider::popEffect()
{
	Effect* effect = mEffects.front();
	mEffects.pop();
	return effect;
}

