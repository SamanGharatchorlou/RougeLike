#include "pch.h"
#include "EffectManager.h"

#include "Objects/Effects/EffectTypes/Effect.h"
#include "Objects/Bags/EffectBag.h"


void EffectManager::fillEffectBag(XMLNode effectNode)
{
	mBag.readEffects(effectNode);
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

	std::unordered_map<EffectType, PropertyBag>::const_iterator iter;
	for (iter = mBag.effects().begin(); iter != mBag.effects().end(); iter++)
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