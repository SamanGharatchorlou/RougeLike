#include "pch.h"
#include "EffectHandler.h"

#include "EffectTypes/Effect.h"


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


void EffectHandler::clear(EffectPool* pool)
{
	for (int i = 0; i < mEffects.size(); i++)
	{
		Effect* effect = mEffects[i];
		mExhausted.push(effect);
	}
	mEffects.clear();

	while(mEffectsToAdd.size() > 0)
	{
		Effect* effect = mEffectsToAdd.pop();
		mExhausted.push(effect);
	}
}


// -- Private Functions -- //
void EffectHandler::addQueuedEffects()
{
	while (mEffectsToAdd.size() > 0)
	{
		Effect* effect = mEffectsToAdd.pop();
		effect->init();
		mEffects.push_back(effect);
	}
}



void EffectHandler::endEffect(std::vector<Effect*>::iterator& iter)
{
	Effect* effect = *iter;

	effect->exit();
	effect->clearBaseData();

	iter = mEffects.erase(iter);
	mExhausted.push(effect);
}
