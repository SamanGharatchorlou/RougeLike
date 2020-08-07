#include "pch.h"
#include "EffectHandler.h"
#include "EffectTypes/Effect.h"

#include "EffectPool.h"


// TODO: split this us so everything is delayed added
void EffectHandler::addEffect(Effect* effect)
{
	mEffectsToAdd.push(effect);
}


void EffectHandler::fastUpdate(float dt)
{
	for (Effect* effect : mEffects)
	{
		effect->fastUpdate(dt);
	}
}


void EffectHandler::slowUpdate(float dt)
{
	for (std::vector<Effect*>::iterator iter = mEffects.begin(); iter != mEffects.end();)
	{
		Effect* effect = *iter;

		effect->slowUpdate(dt);

		if (effect->shouldExit())
			endEffect(iter);
		else
			iter++;
	}

	addQueuedEffects();
}


void EffectHandler::render()
{
	for (Effect* effect : mEffects)
	{
		effect->render();
	}
}


void EffectHandler::clear()
{
	for (int i = 0; i < mEffects.size(); i++)
	{
		Effect* effect = mEffects[i];
		mExhausted.push(effect);
	}

	mEffects.clear();
}


void EffectHandler::returnExhaustedEffects(EffectPool* pool)
{
	while (mExhausted.size() > 0)
	{
		Effect* effect = mExhausted.front();
		mExhausted.pop();

		pool->returnObject(effect);
	}
}


// -- Private Functions -- //
void EffectHandler::addQueuedEffects()
{
	while (mEffectsToAdd.size() > 0)
	{
		Effect* effect = mEffectsToAdd.front();
		mEffectsToAdd.pop();

		addEffect(effect);
	}
}



void EffectHandler::endEffect(std::vector<Effect*>::iterator& iter)
{
	Effect* effect = *iter;

	effect->clearBaseData();
	effect->exit();

	iter = mEffects.erase(iter);
	mExhausted.push(effect);
}
