#include "pch.h"
#include "EffectCollider.h"
#include "Objects/Effects/EffectTypes/Effect.h"


Effect* EffectCollider::popEffect()
{
	return mEffects.popFront();
}



void EffectCollider::addNewEffects(UniqueQueue<Effect*>& effects)
{
	while (effects.size() > 0)
	{
		Effect* effect = effects.popFront();
		mEffects.push(effect);
	}
}


void EffectCollider::updateEffectData(const PropertyMap& properties)
{
	UniqueQueue<Effect*>::iterator iter;
	for (iter = mEffects.begin(); iter != mEffects.end(); iter++)
	{
		Effect* effect = *iter;
		effect->fill(properties);
	}
}