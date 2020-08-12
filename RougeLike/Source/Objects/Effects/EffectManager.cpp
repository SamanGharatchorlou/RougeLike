#include "pch.h"
#include "EffectManager.h"

#include "Objects/Effects/EffectTypes/Effect.h"
#include "Utilities/Maps/EffectMap.h"
#include "EffectPool.h"


void EffectManager::fillEffectBag(XMLNode effectNode)
{
	mBag.fill(effectNode);
}


void EffectManager::slowUpdate(float dt)
{
	mHandler.slowUpdate(dt);
	mHandler.returnExhaustedEffects(mPool);
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
}

void EffectManager::returnEffect(Effect* effect)
{
	mPool->returnObject(effect);
}