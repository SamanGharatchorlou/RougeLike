#include "pch.h"
#include "EffectManager.h"

#include "Objects/Effects/EffectTypes/Effect.h"
#include "Objects/Pools/EffectPool.h"



void EffectManager::slowUpdate(float dt)
{
	mHandler.slowUpdate(dt);

	Queue<Effect*>& exhaustedEffects = mHandler.exhaustedEffects();
	while (exhaustedEffects.size() > 0)
		returnEffect(exhaustedEffects.pop());
}

void EffectManager::fastUpdate(float dt)
{
	mHandler.fastUpdate(dt);
}


void EffectManager::addReceivedEffect(Effect* effect)
{
	mHandler.addEffect(effect);
}


UniqueQueue<Effect*> EffectManager::getNewAttackingEffects()
{
	UniqueQueue<Effect*> effects;

	for (int i = 0; i < mAttackingEffects.mTypes.size(); i++)
	{
		Effect* effect = mPool->getObject(mAttackingEffects.mTypes[i]);
		effects.push(effect);
	}

	return effects;
}


void EffectManager::render()
{
	mHandler.render();
}

void EffectManager::clear()
{
	mAttackingEffects.clear();
	mHandler.clear(mPool);

	Queue<Effect*>& exhaustedEffects = mHandler.exhaustedEffects();
	while (exhaustedEffects.size() > 0)
		returnEffect(exhaustedEffects.pop());

	mPool = nullptr;
}


void EffectManager::returnEffect(Effect* effect)
{
	mPool->returnObject(effect, effect->type());
}