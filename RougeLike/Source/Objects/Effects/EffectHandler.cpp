#include "pch.h"
#include "EffectHandler.h"
#include "EffectTypes/Effect.h"

#include "EffectPool.h"


// TODO: split this us so everything is delayed added
void EffectHandler::addEffect(Effect* effect)
{
	if (!mDelayedAdd)
	{
		effect->init();
		mEffects.push_back(effect);
	}
	else
	{
		mEffectsToAdd.push(effect);
	}
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
	mDelayedAdd = true;

	for (std::vector<Effect*>::iterator iter = mEffects.begin(); iter != mEffects.end();)
	{
		Effect* effect = *iter;

		effect->slowUpdate(dt);

		if (effect->shouldExit())
		{
			effect->exit();
			iter = mEffects.erase(iter);

			mExhausted.push(effect);
		}
		else
		{
			iter++;
		}
	}

	mDelayedAdd = false;

	// Effects added to mEffects during the for loop can invalidate the iterator
	while (mEffectsToAdd.size() > 0)
	{
		Effect* effect = mEffectsToAdd.front();
		mEffectsToAdd.pop();

		addEffect(effect);
	}
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
	// DO NOT NEW OR DELETE EFFECT, POOL HANDLES THIS STUFF
	//for (std::vector<Effect*>::iterator iter = mEffects.begin(); iter != mEffects.end();)
	//{
	//	Effect* effect = *iter;

	//	effect->exit();

	//	delete effect;
	//	iter = mEffects.erase(iter);
	//}
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