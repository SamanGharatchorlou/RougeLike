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


void EffectManager::addEffect(Effect* effect)
{
	mHandler.addEffect(effect);
}


void EffectManager::render()
{
	mHandler.render();
}

void EffectManager::clear()
{
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