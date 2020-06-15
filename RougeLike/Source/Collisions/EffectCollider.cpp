#include "pch.h"
#include "EffectCollider.h"

#include "Objects/Effects/Effect.h"


Effect* EffectCollider::popEffect()
{
	Effect* effect = mEffects.front();
	mEffects.pop();
	return effect;
}