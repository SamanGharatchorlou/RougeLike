#include "pch.h"
#include "EffectHandler.h"

#include "Objects/Actor.h"
#include "Effect.h"


void EffectHandler::addEffect(Effect* effect)
{
	effect->set(mActor);
	effect->init();
	mEffects.push_back(effect);
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
		{
			effect->exit();

			delete effect;
			iter = mEffects.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}


void EffectHandler::clear()
{
	for (std::vector<Effect*>::iterator iter = mEffects.begin(); iter != mEffects.end();)
	{
		Effect* effect = *iter;

		effect->exit();

		delete effect;
		iter = mEffects.erase(iter);
	}
}