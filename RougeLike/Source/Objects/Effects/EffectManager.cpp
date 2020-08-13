#include "pch.h"
#include "EffectManager.h"

#include "Objects/Effects/EffectTypes/Effect.h"
#include "Utilities/Maps/EffectMap.h"
#include "Objects/Pools/EffectPool.h"


void EffectManager::fillEffectBag(XMLNode effectNode)
{
	mBag.fill(effectNode);
}


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


std::queue<Effect*> EffectManager::getAttackingEffects()
{
	std::queue<Effect*> effects;

	for(EffectMap::iterator iter = mBag.begin(); iter != mBag.end(); iter++)
	{
		Effect* effect = mPool->getObject(iter->first);
		effect->fill(iter->second);

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
	mBag.empty();
	mHandler.clear(mPool);

	Queue<Effect*>& exhaustedEffects = mHandler.exhaustedEffects();
	while (exhaustedEffects.size() > 0)
		returnEffect(exhaustedEffects.pop());
}


void EffectManager::returnEffect(Effect* effect)
{
	mPool->returnObject(effect, effect->type());
}